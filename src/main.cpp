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

    Serial.println("MQTT CONNECTING...");

    bool result =
        mqttClient.connect("WeatherStation");
        mqttClient.connect(
            cfg.stationName.c_str(),
            cfg.mqttUser.c_str(),
            cfg.mqttPassword.c_str());

            Serial.print("MQTT RESULT=");
            Serial.println(result);

            Serial.print("MQTT STATE=");
            Serial.println(mqttClient.state());

            Serial.print("MQTT CONNECTED FLAG=");
            Serial.println(mqttClient.connected());

            if (mqttClient.connected())
{
    Serial.println("PUBLISH TEST");

    bool pubResult =
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

    Serial.print("PUBLISH RESULT=");
    Serial.println(pubResult);
}


    if (result)
    {
        mqttClient.publish(
            "weather/status",
            "online",
            true);

        Serial.println("MQTT CONNECTED");
    }
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

    mqttClient.loop();
}