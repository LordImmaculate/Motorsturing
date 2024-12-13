#include <LiquidCrystal_I2C.h>

#define POTPIN A0
#define laserSensor 2

LiquidCrystal_I2C lcd(0x27, 16, 2); // adres 0x27, 16 kolommen, 2 rijen

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");
    pinMode(POTPIN, INPUT);
    pinMode(laserSensor, INPUT);
}

void loop()
{
    lcd.clear();
    bool laser = digitalRead(laserSensor);
    lcd.setCursor(0, 0);
    if(laser)
        lcd.print("Laser aan");
    else
        lcd.print("Laser uit");
    int pot = analogRead(POTPIN);
    lcd.setCursor(0, 1);
    lcd.print(pot);
}