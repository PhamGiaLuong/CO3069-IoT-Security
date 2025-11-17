#include "mqttClient.h"

// MQTT config
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;     
const char* mqtt_topic = "hcmut/co3069/iot_security/data";

WiFiClient espClient;
PubSubClient client(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    printLog("MQTT: Message arrived [%s]", topic);
}

void mqttReconnect() {
    while (!client.connected()) {
        printLog("MQTT: Attempting connection...");
        
        String clientId = "esp32-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            printLog("MQTT: Connected to broker [%s]!", mqtt_server);
            client.subscribe("hcmut/co3069/iot_security/control");
        } else {
            printLog("MQTT: Connect failed, rc=%d. Retrying in 5s...", client.state());
            vTaskDelay(pdMS_TO_TICKS(5000));
            return;
        }
    }
}

void initMQTT() {
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);
    log_printf("MQTT: Client initialized. Server: %s", mqtt_server);
}

void handleMQTTConnection() {
    if (!getWifiConnectionStatus()) {
        if(client.connected()) {
            printLog("MQTT: Wi-Fi lost. Disconnecting MQTT.");
            client.disconnect();
        }
        return;
    }

    if (!client.connected()) {
        mqttReconnect();
    }
    
    if (client.connected()) {
        client.loop();
    }
}

bool mqttPublish(const char* topic, const char* payload) {
    if (!client.connected()) {
        printLog("MQTT: Not connected. Cannot publish message.");
        return false;
    }
    
    if (client.publish(topic, payload)) {
        printLog("MQTT: Message published to [%s]: \n ---> %s", topic, payload);
        return true;
    } else {
        printLog("MQTT: Failed to publish message.");
        return false;
    }
}