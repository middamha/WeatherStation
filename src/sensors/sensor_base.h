#pragma once

class SensorBase
{
public:
    virtual bool begin() = 0;

    virtual void update() = 0;

    virtual bool isAvailable() = 0;
};
