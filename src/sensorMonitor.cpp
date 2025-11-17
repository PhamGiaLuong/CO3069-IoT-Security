#include "sensorMonitor.h"

DHT dht(DHTPIN, DHTTYPE);

void readSensorTask(void *pvParameters){
	TickType_t lastWakeTime = xTaskGetTickCount();
  	const TickType_t period = pdMS_TO_TICKS(1000 * 10);
    dht.begin();

    while (1){
        dht.read();
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if(isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
        }

        setDHTData(temperature, humidity);

        // Print the results
        printLog("Humidity: %.2f% - Temperature: %.2f°C", humidity, temperature);
        
		vTaskDelayUntil(&lastWakeTime, period);
    }
}

void randomDataTask(void *pvParameters) {
	TickType_t lastWakeTime = xTaskGetTickCount();
  	const TickType_t period = pdMS_TO_TICKS(1000 * 10);

    while (1){
        float temperature = 25 + (float)random(0, 500) / 100.0;
        float humidity = 70 + (float)random(0, 1000) / 100.0;

        setDHTData(temperature, humidity);

        // Print the results
        printLog("Humidity: %.2f%% - Temperature: %.2f°C", humidity, temperature);

        char msgBuffer[150];
        snprintf(msgBuffer, sizeof(msgBuffer), 
            "{\"device_id\": \"esp32_co3069_01\", \"temperature\": %.2f, \"humidity\": %.2f, \"status\": \"unsecure\"}", 
            temperature, humidity);
        
        mqttPublish(MQTT_TOPIC, msgBuffer);
        
		vTaskDelayUntil(&lastWakeTime, period);
    }
}