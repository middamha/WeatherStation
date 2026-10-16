#include <Arduino.h>

#include "config/config_manager.h"
#include "system/logger.h"
#include "wifi/wifi_manager.h"
#include "web/web_manager.h"

#include <WiFi.h>
#include <PubSubClient.h>

ConfigManager configManager;
WiFiManager wifiManager;
WebManager webManager;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

uint32_t lastPublish = 0;

void publishSystemTopics()
{
    mqttClient.publish(
        "weather/status",
        "online",
        true);

    mqttClient.publish(
        "weather/system/ip",
        WiFi.localIP().toString().c_str(),
        true);

    mqttClient.publish(
        "weather/system/rssi",
        String(WiFi.RSSI()).c_str(),
        true);

    mqttClient.publish(
        "weather/system/heap",
        String(ESP.getFreeHeap()).c_str(),
        true);

    mqttClient.publish(
        "weather/system/psram",
        String(ESP.getFreePsram()).c_str(),
        true);

    mqttClient.publish(
        "weather/system/uptime",
        String(millis() / 1000).c_str(),
        true);
}

bool connectMQTT()
{
    AppConfig& cfg =
        configManager.getConfig();

    if (mqttClient.connected())
    {
        return true;
    }

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

    Serial.print("MQTT RESULT=");
    Serial.println(result);

    Serial.print("MQTT STATE=");
    Serial.println(mqttClient.state());

    Serial.print("MQTT CONNECTED FLAG=");
    Serial.println(mqttClient.connected());

    if (mqttClient.connected())
    {
        Serial.println("MQTT CONNECTED");

        publishSystemTopics();

        return true;
    }

    return false;
}

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

        return;
    }

    Logger::info("CONFIG FOUND");

    if (!wifiManager.beginSTA(
            cfg.wifiSSID,
            cfg.wifiPassword))
    {
        Logger::error("WIFI FAILED");

        wifiManager.beginAP();

        webManager.begin(&configManager);

        return;
    }

    webManager.begin(&configManager);

    Serial.println("MQTT CONFIG");

    Serial.print("Host: ");
    Serial.println(cfg.mqttHost);

    Serial.print("Port: ");
    Serial.println(cfg.mqttPort);

    Serial.print("User: ");
    Serial.println(cfg.mqttUser);

    Serial.print("Station: ");
    Serial.println(cfg.stationName);

    mqttClient.setServer(
        cfg.mqttHost.c_str(),
        cfg.mqttPort);

    connectMQTT();
}

void loop()
{
    wifiManager.loop();

    if (!mqttClient.connected())
    {
        static uint32_t lastReconnect = 0;

        if (millis() - lastReconnect > 5000)
        {
            lastReconnect = millis();

            connectMQTT();
        }
    }

    mqttClient.loop();

    if (mqttClient.connected())
    {
        if (millis() - lastPublish > 60000)
        {
            lastPublish = millis();

            publishSystemTopics();
        }
    }
}