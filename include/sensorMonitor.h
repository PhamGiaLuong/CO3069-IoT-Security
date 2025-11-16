#ifndef __SENSOR_MONITOR__
#define __SENSOR_MONITOR__

#include <Arduino.h>
#include "DHT.h"
#include "global.h"

#define DHTPIN 22
#define DHTTYPE DHT11

void readSensorTask(void *pvParameters);
void randomDataTask(void *pvParameters);

#endif