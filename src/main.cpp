#include "global.h"

#include "accessPoint.h"


void setup() {
    Serial.begin(115200);

    // xTaskCreate(led_blinky, "Task LED Blink", 2048, NULL, 2, NULL);
    // xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
    // xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 2048, NULL, 2, NULL);
    xTaskCreate(webServerTask, "Task Main Server", 8192, NULL, 2, NULL);
    // xTaskCreate(tiny_ml_task, "Tiny ML Task", 2048, NULL, 2, NULL);
    // xTaskCreate(coreiot_task, "CoreIOT Task", 4096, NULL, 2, NULL);
}

void loop() {
  
}