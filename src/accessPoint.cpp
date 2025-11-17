#include "accessPoint.h"

WebServer server(80);

bool isAPMode = true;
bool connecting = false;
unsigned long connectStartMs = 0;
int connectRetryCount = 0;
ConnectState connectState = IDLE;
unsigned long revertApTimer = 0;
bool switchingToSta = false;
unsigned long switchStaTimer = 0;
unsigned long lastBlinkMs = 0;
bool ledState = false;

void connectToWiFi();

// ========== Handlers ==========
void handleRoot() {
	server.send(200, "text/html", getMainPageHtml());
}

void handleStatus() {
	String json = "{\"status\":\"idle\"}";

	switch(connectState) {
		case CONNECTING:
		json = "{\"status\":\"connecting\", \"attempt\":" + String(connectRetryCount + 1) + "}";
		break;
		case SUCCESS:
		json = "{\"status\":\"success\", \"ip\":\"" + WiFi.localIP().toString() + "\"}";
		connectState = IDLE; 
		break;
		case FAILED:
		json = "{\"status\":\"failed\"}";
		connectState = IDLE;
		break;
		case IDLE:
		break;
	}

	server.send(200, "application/json", json);
}

void handleConnect() {
	String n_ssid = server.arg("ssid");
	String n_pass = server.arg("pass");
	if (n_ssid.length() > 0) {
		setWifiSsid(n_ssid);
		setWifiPassword(n_pass);
		server.send(200, "text/plain", "OK");
		
		isAPMode = false;
		connecting = true;
		connectStartMs = millis();
		connectRetryCount = 0;
		connectState = CONNECTING;
		
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
	connectState = IDLE;
}

void connectToWiFi() {
	String w_ssid = getWifiSsid();
	String w_pass = getWifiPassword();
	printLog("Connecting to WiFi: '%s'", w_ssid);
	WiFi.mode(WIFI_AP_STA);
	WiFi.begin(w_ssid.c_str(), w_pass.c_str());
	connectStartMs = millis();
}

void setupServer() {
	server.on("/", HTTP_GET, handleRoot);
	server.on("/connect", HTTP_POST, handleConnect);
	server.on("/status", HTTP_GET, handleStatus);
	server.begin();
    printLog("Web Server started.");
}

// ========== Main task ==========
void webServerTask(void *pvParameters) {
	pinMode(BOOT_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
	startAP();
	setupServer();
	while (1) {
		server.handleClient();
		handleMQTTConnection();
		// Handle led effect
		if (getWifiConnectionStatus() == false) {
			if (millis() - lastBlinkMs > 250) { 
				lastBlinkMs = millis();
				ledState = !ledState;
				digitalWrite(LED_PIN, ledState);
				// Serial.print("Led state: ");
				// Serial.println(ledState);
			}
			} else {
			digitalWrite(LED_PIN, HIGH);
		}
		// Handle BOOT button
		if (digitalRead(BOOT_PIN) == LOW) {
			vTaskDelay(pdMS_TO_TICKS(100)); // Debounce
			if (digitalRead(BOOT_PIN) == LOW) {
        		printLog("Button Pressed: Force AP Mode");
				if (!isAPMode) {
				startAP();
				}
				while(digitalRead(BOOT_PIN) == LOW) vTaskDelay(10);
			}
		}
		// Connect wifi
		if (connecting) {
			if (WiFi.status() == WL_CONNECTED) {
		        printLog("WiFi Connected! IP: %s", WiFi.localIP().toString().c_str());
				
				setWifiConnectionStatus(true);
				connecting = false; 
				connectRetryCount = 0;
				connectState = SUCCESS;
				switchingToSta = true;
				switchStaTimer = millis();
			} else if (millis() - connectStartMs > 5000) { 
				connectRetryCount++;
				
				if (connectRetryCount < 3) {
        			printLog("Connection failed. Retrying... (Attempt %d/3)", connectRetryCount + 1);
					connectStartMs = millis();
					connectToWiFi();
				} else {
					// Try 3 times => failed
        			printLog("Connection failed after 3 attempts. Reverting to AP Mode.");
					startAP();
					connectState = FAILED;
				}
			}
		}
		if (switchingToSta) {
			if (millis() - switchStaTimer > 5000) {
				isAPMode = false;
        		printLog("Switching to STA-Only mode.");
				WiFi.mode(WIFI_STA);
				switchingToSta = false;
			}
		}
		if (!isAPMode && !connecting && WiFi.status() != WL_CONNECTED) {
			if(getWifiConnectionStatus() == true) {
        		printLog("WiFi connection lost.");
			}
			setWifiConnectionStatus(false);
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}