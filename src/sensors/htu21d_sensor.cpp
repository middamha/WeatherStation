#include "htu21d_sensor.h"

#include <Arduino.h>
#include <Wire.h>

bool HTU21DSensor::begin()
{
    Wire.begin(8, 9);

    available =
        htu21d.begin();

    if (!available)
    {
        Serial.println(
            "HTU21D NOT FOUND");

        return false;
    }

    Serial.println(
        "HTU21D DETECTED");

    return true;
}

void HTU21DSensor::setInterval(
    uint32_t seconds)
{
    updateIntervalMs =
        seconds * 1000UL;
}

void HTU21DSensor::update()
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

    temperature =
        htu21d.readTemperature();

    humidity =
        htu21d.readHumidity();

    newMeasurement = true;

    Serial.println(
        "HTU21D MEASUREMENT");

    Serial.print("HTU TEMP: ");
    Serial.println(temperature);

    Serial.print("HTU HUM: ");
    Serial.println(humidity);
}

bool HTU21DSensor::hasNewMeasurement()
{
    if (!newMeasurement)
    {
        return false;
    }

    newMeasurement = false;

    return true;
}

void HTU21DSensor::publish(
    MQTTManager* mqtt)
{
    mqtt->publishSensor(
        "weather/htu21d/temp",
        temperature);

    mqtt->publishSensor(
        "weather/htu21d/humidity",
        humidity);

    Serial.println(
        "HTU21D MQTT PUBLISHED");
}

bool HTU21DSensor::isAvailable()
{
    return available;
}

float HTU21DSensor::getTemperature()
{
    return temperature;
}

float HTU21DSensor::getHumidity()
{
    return humidity;
}