#include "config_manager.h"

#include <Preferences.h>

Preferences preferences;

bool ConfigManager::begin()
{
    preferences.begin("weather", false);
    return true;
}

bool ConfigManager::load()
{
    config.wifiSSID =
        preferences.getString("ssid", "");

    config.wifiPassword =
        preferences.getString("wifipass", "");

    config.mqttHost =
        preferences.getString("mqtthost", "");

    config.mqttPort =
        preferences.getUShort("mqttport", 1883);

    config.mqttUser =
        preferences.getString("mqttuser", "");

    config.mqttPassword =
        preferences.getString("mqttpass", "");

    config.stationName =
        preferences.getString(
            "station",
            "WeatherStation");

    return true;
}

bool ConfigManager::save()
{
    preferences.putString(
        "ssid",
        config.wifiSSID);

    preferences.putString(
        "wifipass",
        config.wifiPassword);

    preferences.putString(
        "mqtthost",
        config.mqttHost);

    preferences.putUShort(
        "mqttport",
        config.mqttPort);

    preferences.putString(
        "mqttuser",
        config.mqttUser);

    preferences.putString(
        "mqttpass",
        config.mqttPassword);

    preferences.putString(
        "station",
        config.stationName);

    return true;
}

bool ConfigManager::clear()
{
    preferences.clear();
    return true;
}

bool ConfigManager::isConfigured()
{
    return !config.wifiSSID.isEmpty();
}

AppConfig& ConfigManager::getConfig()
{
    return config;
}