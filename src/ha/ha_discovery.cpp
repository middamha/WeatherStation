#include "ha_discovery.h"

String HADiscovery::createDeviceBlock(
    ConfigManager* config)
{
    AppConfig& cfg =
        config->getConfig();

    String json;

    json += "\"device\":{";
    json += "\"identifiers\":[\"weatherstation_main\"],";
    json += "\"name\":\"";
    json += cfg.stationName;
    json += "\",";
    json += "\"manufacturer\":\"ESP32\",";
    json += "\"model\":\"ESP32-S3 N16R8\",";
    json += "\"sw_version\":\"v0.4-home-assistant\"";
    json += "}";

    return json;
}

// System Entities  

void HADiscovery::publishSystemEntities(
    MQTTManager* mqtt,
    ConfigManager* config)
{
    Serial.println(
        "HA SYSTEM DISCOVERY");

    String device =
        createDeviceBlock(config);

    String payload;

    // STATUS

    payload =
        "{"
        "\"name\":\"Status\","
        "\"unique_id\":\"weatherstation_status\","
        "\"state_topic\":\"weather/status\","
        "\"payload_on\":\"online\","
        "\"payload_off\":\"offline\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/binary_sensor/weatherstation_status/config",
        payload.c_str());

    // RSSI

    payload =
        "{"
        "\"name\":\"RSSI\","
        "\"unique_id\":\"weatherstation_rssi\","
        "\"state_topic\":\"weather/system/rssi\","
        "\"unit_of_measurement\":\"dBm\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_rssi/config",
        payload.c_str());

    // HEAP

    payload =
        "{"
        "\"name\":\"Heap\","
        "\"unique_id\":\"weatherstation_heap\","
        "\"state_topic\":\"weather/system/heap\","
        "\"unit_of_measurement\":\"B\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_heap/config",
        payload.c_str());

    // PSRAM

    payload =
        "{"
        "\"name\":\"PSRAM\","
        "\"unique_id\":\"weatherstation_psram\","
        "\"state_topic\":\"weather/system/psram\","
        "\"unit_of_measurement\":\"B\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_psram/config",
        payload.c_str());

    // UPTIME

    payload =
        "{"
        "\"name\":\"Uptime\","
        "\"unique_id\":\"weatherstation_uptime\","
        "\"state_topic\":\"weather/system/uptime\","
        "\"unit_of_measurement\":\"s\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_uptime/config",
        payload.c_str());
}

// BME280 Entities

void HADiscovery::publishBME280(
    MQTTManager* mqtt,
    ConfigManager* config)
{
    Serial.println(
        "HA BME280 DISCOVERY");

    String device =
        createDeviceBlock(config);

    String payload;

    // TEMPERATURE

    payload =
        "{"
        "\"name\":\"BME280 Temperature\","
        "\"unique_id\":\"weatherstation_bme280_temp\","
        "\"state_topic\":\"weather/bme280/temp\","
        "\"unit_of_measurement\":\"°C\","
        "\"device_class\":\"temperature\","
        "\"state_class\":\"measurement\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_bme280_temp/config",
        payload.c_str());

    // PRESSURE

    payload =
        "{"
        "\"name\":\"BME280 Pressure\","
        "\"unique_id\":\"weatherstation_bme280_pressure\","
        "\"state_topic\":\"weather/bme280/pressure\","
        "\"unit_of_measurement\":\"hPa\","
        "\"device_class\":\"atmospheric_pressure\","
        "\"state_class\":\"measurement\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_bme280_pressure/config",
        payload.c_str());

    // HUMIDITY

    payload =
        "{"
        "\"name\":\"BME280 Humidity\","
        "\"unique_id\":\"weatherstation_bme280_humidity\","
        "\"state_topic\":\"weather/bme280/humidity\","
        "\"unit_of_measurement\":\"%\","
        "\"device_class\":\"humidity\","
        "\"state_class\":\"measurement\",";

    payload += device;
    payload += "}";

    mqtt->publish(
        "homeassistant/sensor/weatherstation_bme280_humidity/config",
        payload.c_str());
}