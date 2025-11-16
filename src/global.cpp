#include "global.h"

String apSsid = "IoT-Security-AP";
String apPassword = "mmanm251";
String wifiSsid = "";
String wifiPassword = "";
boolean isWifiConnected = false;

const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Wi-Fi Settings</title>
  <style>
    :root {
      --primary: #00bad1;
      --light: #d6f4f8;     
      --dark: #d6f4f8;
      --bg: #f6f8fa;
      --text: #333;
      --radius: 8px;
      --shadow: 0 2px 5px rgba(214, 244, 248, 1);
    }

    body {
      font-family: "Segoe UI", Roboto, sans-serif;
      background: var(--bg);
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      min-height: 100vh;
    }

    .container {
      width: 95%;
      max-width: 450px;
      background: #fff;
      border-radius: var(--radius);
      box-shadow: var(--shadow);
      margin-top: 50px;
      padding: 16px;
      box-sizing: border-box;
      animation: fadeIn 0.5s ease;
    }

    h2 {
      text-align: center;
      color: var(--primary);
      margin-bottom: 25px;
    }

    .form-group {
      width: 100%;
      margin-bottom: 20px;
    }

    label {
      display: block;
      font-size: 0.9rem;
      color: var(--text);
      margin-bottom: 5px;
      font-weight: 500;
    }

    input {
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
      box-shadow: 0 0 4px var(--primary);
      outline: none;
    }
    
    button {
      padding: 10px 18px;
      font-size: 16px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: all 0.25s ease;
    }
    
    #connectBtn {
      background-color: var(--light);
      color: var(--primary);
      width: 100%;
    }

    #connectBtn:hover {
      box-shadow: var(--shadow);
      background: var(--dark);
    }

    #backBtn {
      background-color: #e0e0e0;
      color: #333;
      margin-top: 10px;
      width: 100%;
    }

    #backBtn:hover {
      background-color: #ccc;
    }

    .status {
      margin-top: 15px;
      text-align: center;
      font-size: 0.9rem;
      color: #777;
    }

    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(10px); }
      to { opacity: 1; transform: translateY(0); }
    }

    @media (max-width: 480px) {
      .container { padding: 20px; margin-top: 30px; }
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
        <input type="password" id="password" name="password" placeholder="Enter password" required>
      </div>

      <button id="connectBtn" type="submit">Connect</button>
      <button id="backBtn" type="button" onclick="window.location='/'">Back</button>
      <p class="status" id="statusText">Waiting for input...</p>
    </form>
  </div>

  <script>
    const form = document.getElementById("wifiForm");
    const statusText = document.getElementById("statusText");

    form.addEventListener("submit", async (e) => {
      e.preventDefault();
      const ssid = document.getElementById("ssid").value;
      const password = document.getElementById("password").value;
      statusText.textContent = "Connecting...";

      // Chuẩn bị dữ liệu để gửi trong body
      const formData = new URLSearchParams();
      formData.append('ssid', ssid);
      formData.append('pass', password);

      try {
        const res = await fetch('/connect', {
          method: 'POST', //
          headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
          },
          body: formData
        });

        if (res.ok) {
          statusText.textContent = "Credentials sent. Awaiting connection status...";
        } else {
          statusText.textContent = "Failed to send credentials.";
          statusText.style.color = "red";
        }
      } catch (err) {
        statusText.textContent = "Error connecting.";
        statusText.style.color = "red";
      }
    });
  </script>
</body>
</html>

)rawliteral";


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