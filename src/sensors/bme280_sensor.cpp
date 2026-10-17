#include "bme280_sensor.h"

#include <Arduino.h>
#include <Wire.h>

bool BME280Sensor::begin()
{
    Wire.begin(8, 9);

    available =
        bme.begin(0x76);

    if (!available)
    {
        Serial.println(
            "BME280 NOT FOUND");

        return false;
    }

    bme.setSampling(
        Adafruit_BME280::MODE_FORCED,

        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,

        Adafruit_BME280::FILTER_OFF);

    Serial.println(
        "BME280 DETECTED");

    return true;
}

void BME280Sensor::setInterval(
    uint32_t seconds)
{
    updateIntervalMs =
        seconds * 1000UL;
}

void BME280Sensor::update()
{
    if (!available)
    {
        return;
    }

    if (!firstMeasurement)
    {
        if (millis() - lastMeasurement <
            updateIntervalMs)
        {
            return;
        }
    }

    firstMeasurement = false;

    lastMeasurement = millis();

    if (!bme.takeForcedMeasurement())
    {
        Serial.println(
            "BME280 MEASUREMENT FAILED");

        return;
    }

    temperature =
        bme.readTemperature();

    pressure =
        bme.readPressure() / 100.0f;

    humidity =
        bme.readHumidity();

    newMeasurement = true;

    Serial.println(
        "BME280 MEASUREMENT");

    Serial.print("TEMP: ");
    Serial.println(temperature);

    Serial.print("PRESSURE: ");
    Serial.println(pressure);

    Serial.print("HUMIDITY: ");
    Serial.println(humidity);
}

bool BME280Sensor::hasNewMeasurement()
{
    if (!newMeasurement)
    {
        return false;
    }

    newMeasurement = false;

    return true;
}

bool BME280Sensor::isAvailable()
{
    return available;
}

float BME280Sensor::getTemperature()
{
    return temperature;
}

float BME280Sensor::getPressure()
{
    return pressure;
}

float BME280Sensor::getHumidity()
{
    return humidity;
}