#include <Arduino.h>

#include "config/config_manager.h"
#include "system/logger.h"
#include "wifi/wifi_manager.h"
#include "web/web_manager.h"

ConfigManager configManager;
WiFiManager wifiManager;
WebManager webManager;

void setup()
{
    Logger::begin();

    Logger::info("BOOT");

    configManager.begin();
    configManager.load();

    AppConfig& cfg =
        configManager.getConfig();

    if (cfg.wifiSSID.isEmpty())
    {
        Logger::info("NO CONFIG");

        wifiManager.beginAP();

        webManager.begin(&configManager);
    }
    else
    {
        Logger::info("CONFIG FOUND");

        if (wifiManager.beginSTA(
                cfg.wifiSSID,
                cfg.wifiPassword))
        {
            webManager.begin(&configManager);
        }
        else
        {
            Logger::error("WIFI FAILED");

            wifiManager.beginAP();

            webManager.begin(&configManager);
        }
    }
}

void loop()
{
    wifiManager.loop();
}