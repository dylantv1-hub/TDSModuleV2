#include "KeyestudioTDS.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define TdsSensorPin 0
#define oneWireBus 2

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

KeyestudioTDS tds;
float temperatureC = 25;

OneWire oneWire(oneWireBus);

DallasTemperature sensors(&oneWire);

void setup()
{
    Serial.begin(115200);
    tds.setPin(TdsSensorPin);
    tds.setAref(3.3);
    tds.setAdcRange(4096);
    tds.begin();

    sensors.begin();

    lcd.init(); // initialize the lcd
	lcd.backlight();
}

void loop()
{
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    Serial.print("Temp: ");
    Serial.print(temperatureC);
    Serial.println("ºC");
   
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(temperatureC);
    lcd.print((char)223);
    lcd.print("C");

    tds.setTemperature(temperatureC);
    tds.update();
    Serial.print("TDS Value:");
    Serial.print(tds.getTdsValue(), 0);
    Serial.println("ppm");
    Serial.println();

    lcd.setCursor(0,1);
    lcd.print("TDS Val: ");
    lcd.print(tds.getTdsValue(), 0);
    lcd.print("ppm   ");

    delay(1);
}
