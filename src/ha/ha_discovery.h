#pragma once

#include "../config/config_manager.h"
#include "../mqtt/mqtt_manager.h"

class HADiscovery
{
public:
    void publish(
        MQTTManager* mqtt,
        ConfigManager* config);

private:
    String createDeviceBlock(
        ConfigManager* config);
};