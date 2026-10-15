#pragma once

#include <Arduino.h>

struct AppConfig
{
    String wifiSSID;
    String wifiPassword;

    String mqttHost;
    uint16_t mqttPort;

    String mqttUser;
    String mqttPassword;

    String stationName;
};

class ConfigManager
{
public:
    bool begin();

    bool load();
    bool save();
    bool clear();

    bool isConfigured();

    AppConfig& getConfig();

private:
    AppConfig config;
};