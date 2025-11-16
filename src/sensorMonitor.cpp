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
        printTimestamp();
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("% - Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
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
		printTimestamp();
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("% - Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
		vTaskDelayUntil(&lastWakeTime, period);
    }
}