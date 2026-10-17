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

    Serial.println("MQTT CONNECTED");

    mqttClient.publish(
        "weather/status",
        "online",
        true);

    publishSystemTopics();

    return true;
}

void MQTTManager::publishSystemTopics()
{
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

bool MQTTManager::publish(
    const char* topic,
    const char* payload,
    bool retained)
{
    if (!mqttClient.connected())
    {
        return false;
    }

    bool result =
        mqttClient.publish(
            topic,
            payload,
            retained);

    Serial.print("MQTT PUBLISH: ");
    Serial.println(topic);

    return result;
}

PubSubClient& MQTTManager::getClient()
{
    return mqttClient;
}

bool MQTTManager::isConnected()
{
    return mqttClient.connected();
}

void MQTTManager::loop()
{
    if (!mqttClient.connected())
    {
        if (millis() - lastReconnect > 5000)
        {
            lastReconnect = millis();

            connect();
        }

        return;
    }

    mqttClient.loop();

    if (millis() - lastPublish > 60000)
    {
        lastPublish = millis();

        publishSystemTopics();
    }
    
}
bool MQTTManager::publishSensor(
    const char* topic,
    float value,
    uint8_t precision)
{
    char buffer[32];

    dtostrf(
        value,
        0,
        precision,
        buffer);

    return publish(
        topic,
        buffer,
        true);
}