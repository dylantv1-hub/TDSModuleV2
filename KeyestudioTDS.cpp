#include "KeyestudioTDS.h"

KeyestudioTDS::KeyestudioTDS()
{
    this->pin = 0;
    this->temperature = 25.0;
    this->analogBufferIndex = 0;
    this->averageVoltage = 0;
    this->tdsValue = 0;
    this->analogSampleTimepoint = 0;
    this->printTimepoint = 0;
    this->isBufferFull = false;
}

void KeyestudioTDS::setPin(int pin){ this->pin = pin; }
void KeyestudioTDS::setTemperature(float temp){ this->temperature = temp; }

void KeyestudioTDS::begin()
{
    pinMode(this->pin, INPUT);
    this->analogSampleTimepoint = millis();
    this->printTimepoint = millis();
}

void KeyestudioTDS::update()
{
    if(millis() - this->analogSampleTimepoint > 40U)
    {
        this->analogSampleTimepoint = millis();
        this->analogBuffer[this->analogBufferIndex] = analogReadMilliVolts(this->pin);
        this->analogBufferIndex++;
        if(this->analogBufferIndex == SCOUNT)
        {
            this->analogBufferIndex = 0;
            this->isBufferFull = true;
        }
    }

    if(millis() - this->printTimepoint > 800U)
    {
        this->printTimepoint = millis();

        if (!isBufferFull) return;

        this->averageVoltage = getMedianNum(this->analogBuffer, SCOUNT) / 1000.0;

        float compensationCoefficient = 1.0 + 0.02 * (this->temperature - 25.0);
        float compensationVoltage = this->averageVoltage / compensationCoefficient;

        this->tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                        - 255.86 * compensationVoltage * compensationVoltage
                        + 857.39 * compensationVoltage) * 0.5;
    }
}

float KeyestudioTDS::getTdsValue(){ return this->tdsValue; }
float KeyestudioTDS::getVoltage(){ return this->averageVoltage; }

int KeyestudioTDS::getMedianNum(int* bArray, int iFilterLen)
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
