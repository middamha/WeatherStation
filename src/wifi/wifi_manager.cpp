#include "wifi_manager.h"

#include <WiFi.h>

void WiFiManager::beginAP()
{
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "ESP32-Weather-Setup",
        "12345678");

    Serial.println();
    Serial.println("ACCESS POINT STARTED");

    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
}

bool WiFiManager::beginSTA(
    const String& ssid,
    const String& password)
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(
        ssid.c_str(),
        password.c_str());

    Serial.print("CONNECTING TO: ");
    Serial.println(ssid);

    uint32_t start = millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - start < 15000)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WIFI CONNECTED");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        return true;
    }

    Serial.println("WIFI FAILED");

    return false;
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP()
{
    return WiFi.localIP().toString();
}

void WiFiManager::loop()
{
}