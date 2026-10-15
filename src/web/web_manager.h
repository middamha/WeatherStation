#pragma once

#include "config/config_manager.h"

class WebManager
{
public:
    void begin(ConfigManager* configManager);

private:
    ConfigManager* config = nullptr;
};