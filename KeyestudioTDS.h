#ifndef KEYESTUDIO_TDS_H
#define KEYESTUDIO_TDS_H

#include "Arduino.h"

#define SCOUNT 30

class KeyestudioTDS
{
public:
    KeyestudioTDS();
    void setPin(int pin);
    void setAref(float value);
    void setAdcRange(float range);
    void setTemperature(float temp);
    void begin();
    void update();
    float getTdsValue();
    float getVoltage();

private:
    int pin;
    float aref;
    float adcRange;
    float temperature;
    int analogBuffer[SCOUNT];
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex;
    float averageVoltage;
    float tdsValue;
    unsigned long analogSampleTimepoint;
    unsigned long printTimepoint;
    int getMedianNum(int* bArray, int iFilterLen);
};

#endif
