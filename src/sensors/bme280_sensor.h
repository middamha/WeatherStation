#pragma once

#include <Adafruit_BME280.h>
#include "sensor_base.h"
#include "../mqtt/mqtt_manager.h"

class BME280Sensor : public SensorBase
{
public:
    bool begin() override;

    void update() override;

    bool isAvailable() override;

    void setInterval(
        uint32_t seconds);

    float getTemperature();

    float getPressure();

    float getHumidity();

    bool hasNewMeasurement();

    void publish(
    MQTTManager* mqtt);


private:
    Adafruit_BME280 bme;

    bool available = false;

    bool firstMeasurement = true;

    bool newMeasurement = false;

    float temperature = 0.0f;

    float pressure = 0.0f;

    float humidity = 0.0f;

    uint32_t lastMeasurement = 0;

    uint32_t updateIntervalMs =
        300000;
};