#include "sensorMonitor.h"
#include "crypto_helper.h"

DHT dht(DHTPIN, DHTTYPE);

// Task đọc cảm biến thật (nếu dùng)
void readSensorTask(void *pvParameters){
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(1000 * 10);
    dht.begin();

    while (1){
        dht.read();
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if(isnan(temperature) || isnan(humidity)) {
            printLog("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
        }

        setDHTData(temperature, humidity);
        printLog("Humidity: %.2f% - Temperature: %.2f°C", humidity, temperature);
        
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

// Task tạo dữ liệu giả lập và gửi đi (Task bạn đang dùng)
void randomDataTask(void *pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(1000 * 10); // Gửi mỗi 10 giây

    while (1){
        // --- 1. TẠO DỮ LIỆU GIẢ (Phần bị thiếu) ---
        float temperature = 25 + (float)random(0, 500) / 100.0;
        float humidity = 70 + (float)random(0, 1000) / 100.0;

        setDHTData(temperature, humidity);

        // In log để kiểm tra
        printLog("Humidity: %.2f%% - Temperature: %.2f°C", humidity, temperature);

        // --- 2. TẠO JSON DOCUMENT ---
        // ArduinoJson v7 sử dụng JsonDocument thay vì StaticJsonDocument
        JsonDocument doc; 
        
        doc["device_id"] = "esp32_co3069_01";
        // serialized() giúp giữ nguyên định dạng số thực với 2 số thập phân
        doc["temperature"] = serialized(String(temperature, 2));
        doc["humidity"] = serialized(String(humidity, 2));
        doc["status"] = "secured"; // Đánh dấu là gói tin đã bảo mật

        // --- 3. SERIALIZE JSON RA CHUỖI ---
        char jsonBuffer[256];
        serializeJson(doc, jsonBuffer);
        String plainJson = String(jsonBuffer);

        // --- 4. MÃ HÓA DỮ LIỆU (AES-128 + Base64) ---
        printLog("Original JSON: %s", plainJson.c_str());
        String encryptedPayload = encryptMessage(plainJson);
        printLog("Encrypted Payload: %s", encryptedPayload.c_str());

        // --- 5. GỬI MQTT ---
        if (encryptedPayload.length() > 0) {
            // Gửi chuỗi ĐÃ MÃ HÓA lên topic
            mqttPublish(MQTT_TOPIC, encryptedPayload.c_str());
        } else {
            printLog("ERROR: Encryption failed!");
        }
        
        vTaskDelayUntil(&lastWakeTime, period);
    }
}