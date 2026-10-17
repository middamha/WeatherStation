#pragma once

#include "../config/config_manager.h"
#include "../mqtt/mqtt_manager.h"

class HADiscovery
{
public:

    void publishSystemEntities(
        MQTTManager* mqtt,
        ConfigManager* config);

    void publishBME280(
        MQTTManager* mqtt,
        ConfigManager* config);

    void publishHTU21D(
        MQTTManager* mqtt,
    ConfigManager* config);

private:
    String createDeviceBlock(
        ConfigManager* config);
};