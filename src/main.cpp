#include "global.h"

#include "accessPoint.h"
#include "sensorMonitor.h"


void setup() {
    Serial.begin(115200);
    xDHTSemaphore = xSemaphoreCreateMutex();
    initMQTT();

    // xTaskCreate(readSensorTask, "Task Read Sensor", 2048, NULL, 2, NULL);
    xTaskCreate(randomDataTask, "Task Random Data", 4096, NULL, 2, NULL);
    xTaskCreate(webServerTask, "Task Web Server", 8192, NULL, 3, NULL);
    // xTaskCreate(coreiot_task, "CoreIOT Task", 4096, NULL, 2, NULL);
}

void loop() {
  
}