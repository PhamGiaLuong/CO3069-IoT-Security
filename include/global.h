#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define DHT_LOCK_TIMEOUT pdMS_TO_TICKS(200)
extern SemaphoreHandle_t xDHTSemaphore;

String getApSsid();
String getApPassword();
String getWifiSsid();
String getWifiPassword();
boolean getWifiConnectionStatus();
const char* getMainPageHtml();
float getTemperatureData();
float getHumidityData();

void setApSsid(const String &ssid);
void setApPassword(const String &pass);
void setWifiSsid(const String &ssid);
void setWifiPassword(const String &pass);
void setWifiConnectionStatus(boolean status);
void setTemperatureData(float temp);
void setHumidityData(float hum);
void setDHTData(float temp, float hum);

void printTimestamp();
void printLog(const char * format, ...);

#endif