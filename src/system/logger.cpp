#include <Arduino.h>
#include "logger.h"

void Logger::begin()
{
    Serial.begin(115200);
}

void Logger::info(const char* message)
{
    Serial.print("[INFO] ");
    Serial.println(message);
}

void Logger::error(const char* message)
{
    Serial.print("[ERROR] ");
    Serial.println(message);
}