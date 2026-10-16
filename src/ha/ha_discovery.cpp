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

void HADiscovery::publish(
    MQTTManager* mqtt,
    ConfigManager* config)
{
    Serial.println("HA DISCOVERY PUBLISH");

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

    Serial.print("DISCOVERY STATUS RESULT=");
    Serial.println(
        mqtt->publish(
            "homeassistant/binary_sensor/weatherstation_status/config",
            payload.c_str()));

    // RSSI

    payload =
        "{"
        "\"name\":\"RSSI\","
        "\"unique_id\":\"weatherstation_rssi\","
        "\"state_topic\":\"weather/system/rssi\","
        "\"unit_of_measurement\":\"dBm\",";

    payload += device;
    payload += "}";

    Serial.print("DISCOVERY RSSI RESULT=");
    Serial.println(
        mqtt->publish(
            "homeassistant/sensor/weatherstation_rssi/config",
            payload.c_str()));

    // HEAP

    payload =
        "{"
        "\"name\":\"Heap\","
        "\"unique_id\":\"weatherstation_heap\","
        "\"state_topic\":\"weather/system/heap\","
        "\"unit_of_measurement\":\"B\",";

    payload += device;
    payload += "}";

    Serial.print("DISCOVERY HEAP RESULT=");
    Serial.println(
        mqtt->publish(
            "homeassistant/sensor/weatherstation_heap/config",
            payload.c_str()));

    // PSRAM

    payload =
        "{"
        "\"name\":\"PSRAM\","
        "\"unique_id\":\"weatherstation_psram\","
        "\"state_topic\":\"weather/system/psram\","
        "\"unit_of_measurement\":\"B\",";

    payload += device;
    payload += "}";

    Serial.print("DISCOVERY PSRAM RESULT=");
    Serial.println(
        mqtt->publish(
            "homeassistant/sensor/weatherstation_psram/config",
            payload.c_str()));

    // UPTIME

    payload =
        "{"
        "\"name\":\"Uptime\","
        "\"unique_id\":\"weatherstation_uptime\","
        "\"state_topic\":\"weather/system/uptime\","
        "\"unit_of_measurement\":\"s\",";

    payload += device;
    payload += "}";

    Serial.print("DISCOVERY UPTIME RESULT=");
    Serial.println(
        mqtt->publish(
            "homeassistant/sensor/weatherstation_uptime/config",
            payload.c_str()));
}