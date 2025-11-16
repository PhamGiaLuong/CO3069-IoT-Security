#ifndef ___ACCESS_POINT__
#define ___ACCESS_POINT__

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "global.h"

#define BOOT_PIN 0

enum ConnectState { IDLE, CONNECTING, SUCCESS, FAILED };

void webServerTask(void *pvParameters);

#endif