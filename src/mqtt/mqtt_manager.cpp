#include "mqtt_manager.h"

#include <Arduino.h>

void MQTTManager::begin(
    ConfigManager* configManager)
{
    config = configManager;

    AppConfig& cfg =
        config->getConfig();

    mqttClient.setServer(
        cfg.mqttHost.c_str(),
        cfg.mqttPort);
}

bool MQTTManager::connect()
{
    if (mqttClient.connected())
    {
        return true;
    }

    AppConfig& cfg =
        config->getConfig();

    Serial.println("MQTT CONNECTING...");

    bool result =
        mqttClient.connect(
            cfg.stationName.c_str(),
            cfg.mqttUser.c_str(),
            cfg.mqttPassword.c_str(),
            "weather/status",
            1,
            true,
            "offline");

    if (!result)
    {
        Serial.print("MQTT STATE=");
        Serial.println(
            mqttClient.state());

        return false;
    }

    Serial.println(
        "MQTT CONNECTED");

    publishSystemTopics();

    return true;
}

void MQTTManager::publishSystemTopics()
{
    mqttClient.publish(
        "weather/status",
        "online",
        true);

    mqttClient.publish(
        "weather/system/ip",
        WiFi.localIP()
            .toString()
            .c_str(),
        true);

    mqttClient.publish(
        "weather/system/rssi",
        String(WiFi.RSSI())
            .c_str(),
        true);

    mqttClient.publish(
        "weather/system/heap",
        String(ESP.getFreeHeap())
            .c_str(),
        true);

    mqttClient.publish(
        "weather/system/psram",
        String(ESP.getFreePsram())
            .c_str(),
        true);

    mqttClient.publish(
        "weather/system/uptime",
        String(millis() / 1000)
            .c_str(),
        true);
}

bool MQTTManager::isConnected()
{
    return mqttClient.connected();
}

void MQTTManager::loop()
{
    mqttClient.loop();

    if (!mqttClient.connected())
    {
        if (millis() - lastReconnect > 5000)
        {
            lastReconnect = millis();

            connect();
        }

        return;
    }

    if (millis() - lastPublish > 60000)
    {
        lastPublish = millis();

        publishSystemTopics();
    }
}