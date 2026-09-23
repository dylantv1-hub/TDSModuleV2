#include <KeyestudioTDS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
KeyestudioTDS tds;                  
OneWire onewire(2);
DallasTemperature tempSensor(&onewire);

float temperatureC = 25;
float tdsVal = 0;
unsigned long printTimer = 0;

void setup()
{
    Serial.begin(115200);   //set to 115200 if using esp32, 9600 if using arduino
    
    tds.setPin(0);          
    tds.begin();            //initialize tds sensor

    tempSensor.begin();        //initialize temperature sensor

    lcd.init();             //initialize the lcd
	lcd.backlight();
}

void loop()
{
    tempSensor.update();
    temperatureC = tempSensor.getTemperature();

    tds.setTemperature(temperatureC);
    tds.update();
    tdsVal = tds.getTdsValue();

    if (millis() - printTimer >= 1000U)
    {
        printTimer = millis();
        Serial.print("Temp:");
        Serial.print(temperatureC);
        Serial.print(",");
        Serial.print("TDS Value:");
        Serial.println(tdsVal, 0);
    
        lcd.setCursor(0,0);
        lcd.print("Temp: ");
        lcd.print(temperatureC);
        lcd.print((char)223);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("TDS: ");
        lcd.print(tdsVal, 0);
        lcd.print("ppm   ");
    }
}
