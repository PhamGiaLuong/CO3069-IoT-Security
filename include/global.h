#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

String getApSsid();
String getApPassword();
String getWifiSsid();
String getWifiPassword();
boolean getWifiConnectionStatus();
const char* getMainPageHtml();

void setApSsid(const String &ssid);
void setApPassword(const String &pass);
void setWifiSsid(const String &ssid);
void setWifiPassword(const String &pass);
void setWifiConnectionStatus(boolean status);

#endif