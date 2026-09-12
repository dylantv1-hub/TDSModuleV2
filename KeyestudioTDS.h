#ifndef KEYESTUDIO_TDS_H
#define KEYESTUDIO_TDS_H

#include "Arduino.h"

#define SCOUNT 30

class KeyestudioTDS
{
public:
    KeyestudioTDS()
    {
        this->pin = 0;
        this->aref = 3.3;
        this->adcRange = 4096.0;
        this->temperature = 25.0;
        this->analogBufferIndex = 0;
        this->averageVoltage = 0;
        this->tdsValue = 0;
        this->analogSampleTimepoint = 0;
        this->printTimepoint = 0;
    }

    void setPin(int pin){ this->pin = pin; }
    void setAref(float value){ this->aref = value; }
    void setAdcRange(float range){ this->adcRange = range; }
    void setTemperature(float temp){ this->temperature = temp; }

    void begin()
    {
        pinMode(this->pin, INPUT);
        this->analogSampleTimepoint = millis();
        this->printTimepoint = millis();
    }

    void update()
    {
        if(millis() - this->analogSampleTimepoint > 40U)
        {
            this->analogSampleTimepoint = millis();
            this->analogBuffer[this->analogBufferIndex] = analogRead(this->pin);
            this->analogBufferIndex++;
            if(this->analogBufferIndex == SCOUNT)
                this->analogBufferIndex = 0;
        }

        if(millis() - this->printTimepoint > 800U)
        {
            this->printTimepoint = millis();
            for(int i = 0; i < SCOUNT; i++)
                this->analogBufferTemp[i] = this->analogBuffer[i];

            this->averageVoltage = getMedianNum(this->analogBufferTemp, SCOUNT) * this->aref / this->adcRange;

            float compensationCoefficient = 1.0 + 0.02 * (this->temperature - 25.0);
            float compensationVoltage = this->averageVoltage / compensationCoefficient;

            this->tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                            - 255.86 * compensationVoltage * compensationVoltage
                            + 857.39 * compensationVoltage) * 0.5;
        }
    }

    float getTdsValue(){ return this->tdsValue; }
    float getVoltage(){ return this->averageVoltage; }

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

    int getMedianNum(int* bArray, int iFilterLen)
    {
        int bTab[iFilterLen];
        for(int i = 0; i < iFilterLen; i++)
            bTab[i] = bArray[i];
        int bTemp;
        for(int j = 0; j < iFilterLen - 1; j++)
        {
            for(int i = 0; i < iFilterLen - j - 1; i++)
            {
                if(bTab[i] > bTab[i+1])
                {
                    bTemp = bTab[i];
                    bTab[i] = bTab[i+1];
                    bTab[i+1] = bTemp;
                }
            }
        }
        if((iFilterLen & 1) > 0)
            return bTab[(iFilterLen - 1) / 2];
        else
            return (bTab[iFilterLen/2] + bTab[iFilterLen/2 - 1]) / 2;
    }
};

#endif
