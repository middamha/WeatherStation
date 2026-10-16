#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

#include "../config/config_manager.h"

class MQTTManager
{
public:
    void begin(ConfigManager* configManager);

    void loop();

    bool connect();

    bool isConnected();

private:
    void publishSystemTopics();

private:
    ConfigManager* config = nullptr;

    WiFiClient wifiClient;

    PubSubClient mqttClient{wifiClient};

    uint32_t lastReconnect = 0;
    uint32_t lastPublish = 0;
};