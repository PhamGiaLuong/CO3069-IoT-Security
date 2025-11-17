#include "global.h"

SemaphoreHandle_t xDHTSemaphore = NULL;

String apSsid = "IoT-Security-AP";
String apPassword = "mmanm251";
String wifiSsid = "";
String wifiPassword = "";
boolean isWifiConnected = false;
float temperature = 0.0f;
float humidity = 0.0f;

const String tempErr = "ERROR: Timeout waiting for Temperature resource!";
const String humErr = "ERROR: Timeout waiting for Humidity resource!";
const String dataErr = "ERROR: Timeout waiting for Temperature & Humidity resource!";

const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Wi-Fi Settings</title>
  <style>
    :root {
      --primary: #007bff;
      --success: #28a745;
      --danger: #dc3545;
      --warning: #ffc107;
      --info: #17a2b8;
      --bg: #f6f8fa;
      --text: #333;
      --radius: 8px;
      --shadow: 0 2px 5px rgba(0, 0, 0, 0.05);
    }

    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
      background: var(--bg);
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      min-height: 100vh;
      color: var(--text);
    }

    .container {
      width: 95%;
      max-width: 450px;
      background: #fff;
      border-radius: var(--radius);
      box-shadow: var(--shadow);
      margin-top: 50px;
      padding: 24px;
      box-sizing: border-box;
      animation: fadeIn 0.5s ease;
    }

    h2 {
      text-align: center;
      color: var(--primary);
      margin-top: 0;
      margin-bottom: 25px;
    }

    .form-group {
      width: 100%;
      margin-bottom: 20px;
    }

    label {
      display: block;
      font-size: 0.9rem;
      margin-bottom: 5px;
      font-weight: 500;
    }

    input[type="text"],
    input[type="password"] {
      box-sizing: border-box;
      width: 100%;
      padding: 10px 12px;
      border: 1px solid #ccc;
      border-radius: var(--radius);
      font-size: 1rem;
      transition: all 0.3s ease;
    }

    input:focus {
      border-color: var(--primary);
      box-shadow: 0 0 5px rgba(0, 123, 255, 0.25);
      outline: none;
    }
    
    button {
      padding: 12px 18px;
      font-size: 16px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: all 0.25s ease;
      width: 100%;
      background-color: var(--primary);
      color: white;
      font-weight: 500;
    }

    button:hover {
      opacity: 0.85;
    }
    
    button:disabled {
      background-color: #ccc;
      cursor: not-allowed;
    }

    /* --- PHẦN STATUS MỚI --- */
    .status {
      margin-top: 20px;
      padding: 12px;
      border-radius: var(--radius);
      font-size: 0.9rem;
      text-align: center;
      display: none; /* Ẩn mặc định */
    }
    .status-info {
      display: block;
      background-color: #e6f7ff;
      border: 1px solid #91d5ff;
      color: #0056b3;
    }
    .status-success {
      display: block;
      background-color: #f6ffed;
      border: 1px solid #b7eb8f;
      color: #389e0d;
    }
    .status-error {
      display: block;
      background-color: #fff1f0;
      border: 1px solid #ffa39e;
      color: #cf1322;
    }
    /* --- Hết phần status mới --- */

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(10px); }
      to { opacity: 1; transform: translateY(0); }
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Wi-Fi Settings</h2>
    <form id="wifiForm">
      <div class="form-group">
        <label for="ssid">Wi-Fi SSID</label>
        <input type="text" id="ssid" name="ssid" placeholder="Enter Wi-Fi name" required>
      </div>

      <div class="form-group">
        <label for="password">Wi-Fi Password</label>
        <input type="password" id="password" name="password" placeholder="Enter password">
      </div>

      <button id="connectBtn" type="submit">Connect</button>
      <p class="status status-info" id="statusText">Waiting for input...</p>
    </form>
  </div>

  <script>
    const form = document.getElementById("wifiForm");
    const statusText = document.getElementById("statusText");
    const connectBtn = document.getElementById("connectBtn");
    let pollInterval = null;

    // --- Hàm helper để cập nhật status ---
    function setStatus(message, type) {
      statusText.textContent = message;
      statusText.className = 'status status-' + type;
    }

    // --- Hàm Polling kiểm tra status ---
    async function checkStatus() {
      try {
        const res = await fetch('/status');
        if (!res.ok) {
          throw new Error('Network response was not ok');
        }
        const data = await res.json();

        switch(data.status) {
          case 'connecting':
            setStatus(`Connecting... (Attempt ${data.attempt}/3)`, 'info');
            break;
          case 'success':
            setStatus(`Success! Device IP: ${data.ip}. You can close this page.`, 'success');
            clearInterval(pollInterval);
            connectBtn.disabled = false;
            connectBtn.textContent = 'Connect';
            break;
          case 'failed':
            setStatus('Connection failed. Please check password and try again.', 'error');
            clearInterval(pollInterval);
            connectBtn.disabled = false;
            connectBtn.textContent = 'Connect';
            break;
          case 'idle':
            setStatus('Waiting for input...', 'info');
            clearInterval(pollInterval);
            connectBtn.disabled = false;
            connectBtn.textContent = 'Connect';
            break;
        }
      } catch (err) {
        // Lỗi này thường xảy ra khi ESP32 AP tắt sau khi kết nối thành công
        // Ta có thể giả định là kết nối thành công nếu lỗi xảy ra khi đang polling
        console.error('Fetch error:', err);
        if (statusText.className.includes('status-info')) { 
          setStatus('Connection to device lost. Please check your network.', 'error');
          clearInterval(pollInterval);
          connectBtn.disabled = false;
          connectBtn.textContent = 'Connect';
        }
      }
    }

    // --- Xử lý Submit Form ---
    form.addEventListener("submit", async (e) => {
      e.preventDefault();
      if (pollInterval) clearInterval(pollInterval); // Dừng polling cũ (nếu có)

      const ssid = document.getElementById("ssid").value;
      const password = document.getElementById("password").value;
      
      setStatus("Sending credentials...", "info");
      connectBtn.disabled = true;
      connectBtn.textContent = 'Connecting...';

      const formData = new URLSearchParams();
      formData.append('ssid', ssid);
      formData.append('pass', password);

      try {
        const res = await fetch('/connect', {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
          body: formData
        });

        if (res.ok) {
          // Bắt đầu Polling sau khi gửi thành công
          setStatus("Credentials sent. Awaiting connection status...", "info");
          pollInterval = setInterval(checkStatus, 200); // Check mỗi 0.2s
        } else {
          setStatus("Failed to send credentials to device.", "error");
          connectBtn.disabled = false;
          connectBtn.textContent = 'Connect';
        }
      } catch (err) {
        setStatus("Error: Could not reach the device.", "error");
        connectBtn.disabled = false;
        connectBtn.textContent = 'Connect';
      }
    });
  </script>
</body>
</html>
)rawliteral";



void printTimestamp() {
    unsigned long now = millis();
    
    unsigned long totalSeconds = now / 1000;
    unsigned long hours = totalSeconds / 3600;
    unsigned long minutes = (totalSeconds % 3600) / 60;
    unsigned long seconds = totalSeconds % 60;
    unsigned long centis = (now % 1000) / 10; 
    
    Serial.printf("%02lu:%02lu:%02lu.%02lu: ", hours, minutes, seconds, centis);
}

void printLog(const char * format, ...) {
    printTimestamp();
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    Serial.println(buffer);
}


String getApSsid() {
    return apSsid;
}

void setApSsid(const String &ssid) {
    apSsid = ssid;
}

String getApPassword() {
    return apPassword;
}

void setApPassword(const String &pass) {
    apPassword = pass;
}

String getWifiSsid() {
    return wifiSsid;
}

void setWifiSsid(const String &ssid) {
    wifiSsid = ssid;
}

String getWifiPassword() {
    return wifiPassword;
}

void setWifiPassword(const String &pass) {
    wifiPassword = pass;
}

boolean getWifiConnectionStatus() {
    return isWifiConnected;
}

void setWifiConnectionStatus(boolean status) {
    isWifiConnected = status;
}

const char* getMainPageHtml() {
    return MAIN_PAGE;
}

float getTemperatureData() {
	float value = 0.0f;
	if (xSemaphoreTake(xDHTSemaphore, DHT_LOCK_TIMEOUT) == pdTRUE) {
		value = temperature;
		xSemaphoreGive(xDHTSemaphore);
	} else {
		printLog("%s", tempErr);
	}
	return value;
}

void setTemperatureData(float temp) {
	if (xSemaphoreTake(xDHTSemaphore, DHT_LOCK_TIMEOUT) == pdTRUE) {
		temperature = temp;
		xSemaphoreGive(xDHTSemaphore);
	} else {
		printLog("%s", tempErr);
	}	
}

float getHumidityData() {
	float value = 0.0f;
	if (xSemaphoreTake(xDHTSemaphore, DHT_LOCK_TIMEOUT) == pdTRUE) {
		value = humidity;
		xSemaphoreGive(xDHTSemaphore);
	} else {
		printLog("%s", humErr);
	}
	return value;
}

void setHumidityData(float hum) {
	if (xSemaphoreTake(xDHTSemaphore, DHT_LOCK_TIMEOUT) == pdTRUE) {
		humidity = hum;
		xSemaphoreGive(xDHTSemaphore);
	} else {
		printLog("%s", humErr);
	}	
}

void setDHTData(float temp, float hum) {
	if (xSemaphoreTake(xDHTSemaphore, DHT_LOCK_TIMEOUT) == pdTRUE) {
		temperature = temp;
		humidity = hum;
		xSemaphoreGive(xDHTSemaphore);
	} else {
		printLog("%s", dataErr);
	}
}