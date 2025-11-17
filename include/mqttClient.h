#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include "global.h"

void initMQTT();
void handleMQTTConnection();
bool mqttPublish(const char* topic, const char* payload);

#endif