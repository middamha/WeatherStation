#pragma once

#include <HTU21D.h>

#include "../mqtt/mqtt_manager.h"
#include "sensor_base.h"

class HTU21DSensor : public SensorBase
{
public:
    bool begin() override;

    void update() override;

    bool isAvailable() override;

    void setInterval(
        uint32_t seconds);

    bool hasNewMeasurement();

    void publish(
        MQTTManager* mqtt);

    float getTemperature();

    float getHumidity();

private:
    HTU21D htu21d;

    bool available = false;

    bool firstMeasurement = true;

    bool newMeasurement = false;

    float temperature = 0.0f;

    float humidity = 0.0f;

    uint32_t lastMeasurement = 0;

    uint32_t updateIntervalMs =
        300000;
};