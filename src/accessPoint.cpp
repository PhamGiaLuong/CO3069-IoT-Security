#include "accessPoint.h"

WebServer server(80);

bool isAPMode = true;
bool connecting = false;
unsigned long connectStartMs = 0;
int connectRetryCount = 0;

// ========== Handlers ==========
void handleRoot() {
  server.send(200, "text/html", getMainPageHtml());
}

void handleConnect() {
  String n_ssid = server.arg("ssid");
  String n_pass = server.arg("pass");

  if (n_ssid.length() > 0) {
    setWifiSsid(n_ssid);
    setWifiPassword(n_pass);

    server.send(200, "text/plain", "Credentials received. Connecting...");
    
    isAPMode = false;
    connecting = true;
    connectStartMs = millis();
    connectRetryCount = 0;
    
    connectToWiFi();
  } else {
    server.send(400, "text/plain", "SSID cannot be empty");
  }
}

// ========== WiFi ==========
void startAP() {
  String apName = getApSsid();
  String apPass = getApPassword();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName.c_str(), apPass.c_str());
  
  Serial.println("--------------------------------");
  Serial.print("AP Started: "); Serial.println(apName);
  Serial.print("IP Address: "); Serial.println(WiFi.softAPIP());
  Serial.println("--------------------------------");

  isAPMode = true;
  connecting = false;
  setWifiConnectionStatus(false);
}

void connectToWiFi() {
  String w_ssid = getWifiSsid();
  String w_pass = getWifiPassword();

  Serial.print("Connecting to WiFi: '");
  Serial.println(w_ssid);
  Serial.println("'");

  WiFi.mode(WIFI_STA);
  WiFi.begin(w_ssid.c_str(), w_pass.c_str());
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.begin();
  Serial.println("Web Server started.");
}

// ========== Main task ==========
void webServerTask(void *pvParameters) {
  pinMode(BOOT_PIN, INPUT_PULLUP);

  startAP();
  setupServer();

  while (1) {
    server.handleClient();

    // Handle BOOT button
    if (digitalRead(BOOT_PIN) == LOW) {
      vTaskDelay(pdMS_TO_TICKS(100)); // Debounce
      if (digitalRead(BOOT_PIN) == LOW) {
        Serial.println("Button Pressed: Force AP Mode");
        if (!isAPMode) {
          startAP();
        }
        while(digitalRead(BOOT_PIN) == LOW) vTaskDelay(10);
      }
    }

    // Connect wifi
    if (connecting) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print("WiFi Connected! IP: ");
        Serial.println(WiFi.localIP());
        
        setWifiConnectionStatus(true);
        isAPMode = false;
        connecting = false; 
        connectRetryCount = 0;
      } else if (millis() - connectStartMs > 10000) { 
        connectRetryCount++;
        
        if (connectRetryCount < 3) {
          Serial.println("Connection failed. Retrying... (Attempt " + String(connectRetryCount + 1) + "/3)");
          connectStartMs = millis();
          connectToWiFi();
        } else {
          // Try 3 times => failed
          Serial.println("Connection failed after 3 attempts. Reverting to AP Mode.");
          startAP();
          connecting = false;
          connectRetryCount = 0;
        }
      }
    }
    
    if (!isAPMode && !connecting && WiFi.status() != WL_CONNECTED) {
        if(getWifiConnectionStatus() == true) {
          Serial.println("WiFi connection lost.");
        }
        setWifiConnectionStatus(false);
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}