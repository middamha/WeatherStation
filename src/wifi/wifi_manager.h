#pragma once

#include <Arduino.h>

class WiFiManager
{
public:
    void beginAP();

    bool beginSTA(
        const String& ssid,
        const String& password);

    bool isConnected();

    String getIP();

    void loop();
};