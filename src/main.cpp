#include <Arduino.h>

#include "config/config_manager.h"
#include "system/logger.h"
#include "wifi/wifi_manager.h"
#include "web/web_manager.h"
#include "mqtt/mqtt_manager.h"
#include "ha/ha_discovery.h"
#include "sensors/bme280_sensor.h"


ConfigManager configManager;
WiFiManager wifiManager;
WebManager webManager;
MQTTManager mqttManager;
HADiscovery haDiscovery;
BME280Sensor bme280;

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

    mqttManager.begin(&configManager);

    mqttManager.connect();
if (mqttManager.isConnected())
{
    haDiscovery.publish(
        &mqttManager,
        &configManager);
}
if (bme280.begin())
{
    bme280.setInterval(
        cfg.sensorIntervalSeconds);

    bme280.update();

    Serial.println(
        "BME280 READY");
}
}

void loop()
{
    wifiManager.loop();

    mqttManager.loop();

    bme280.update();

     if (bme280.hasNewMeasurement())
    {
        mqttManager.publishSensor(
            "weather/bme280/temp",
            bme280.getTemperature());

        mqttManager.publishSensor(
            "weather/bme280/pressure",
            bme280.getPressure());

        mqttManager.publishSensor(
            "weather/bme280/humidity",
            bme280.getHumidity());

        Serial.println(
            "BME280 MQTT PUBLISHED");
    }
}