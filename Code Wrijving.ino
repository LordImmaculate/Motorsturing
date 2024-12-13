// Motorsturing
//

#include <LiquidCrystal_I2C.h>
#define PWMPIN 3
#define LPIN 4
#define RPIN 5

#define LASER1PIN 13
#define LASER2PIN 12
#define SWITCH1PIN 11
#define SWITCH2PIN 10
#define STARTPIN 8
#define RESETPIN 9
#define POTPIN A0

#define start digitalRead(STARTPIN)

#define reset digitalRead(RESETPIN)
#define switch1 digitalRead(SWITCH1PIN)
#define switch2 digitalRead(SWITCH2PIN)
#define sensor1 digitalRead(LASER1PIN)
#define sensor2 digitalRead(LASER2PIN)

#define plankLengte 25

// Library voor LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // adres 0x27, 16 kolommen, 2 rijen

//Zorgt ervoor dat "Klik op RESET" maar 1 keer wordt geprint
bool resetBericht;

void setup()
{
    Serial.begin(9600);
    pinMode(PWMPIN, OUTPUT);
    pinMode(LPIN, OUTPUT);
    pinMode(RPIN, OUTPUT);

    pinMode(LASER1PIN, INPUT);
    pinMode(LASER2PIN, INPUT);
    pinMode(SWITCH1PIN, INPUT);
    pinMode(SWITCH2PIN, INPUT);
    pinMode(STARTPIN, INPUT);
    pinMode(RESETPIN, INPUT);

    // Start de LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
}

void loop()
{
    if (digitalRead(SWITCH1PIN))
    {
        lcdPrint("Druk op START", 0, true);
        resetBericht = false;
        while (!start);
        lcdPrint("START gedrukt", 0, true);
        traagOmhoog();
        while (start && !sensor1 && !switch2) {}
        stop();

        if (start && !switch2)
        {
            unsigned long tStart = millis();
            while (!sensor2);
            unsigned long tStop = millis();
            unsigned int tVal = tStop - tStart;
            lcdPrint("Fl: " + String(berekeningFl()), 0, true);
            lcdPrint("Fd: " + String(berekeningFd(tVal)), 1, false);
            delay(5000);
        }
    }
    else if (!resetBericht)
    {
        resetBericht = true;
        lcdPrint("Klik op RESET", 0, true);
    }
    else if (reset)
    {
        resetFunc();
    }
}

void traagOmhoog()
{
    Serial.println("Traag omhoog");
    analogWrite(PWMPIN, 100);
    digitalWrite(LPIN, LOW);
    digitalWrite(RPIN, HIGH);
}

void stop()
{
    Serial.println("Stop");
    analogWrite(PWMPIN, 0);
    digitalWrite(LPIN, LOW);
    digitalWrite(RPIN, LOW);
}

void resetFunc()
{
    Serial.println("Reset gedrukt");
    while (!switch1)
    {
        analogWrite(PWMPIN, 100);
        digitalWrite(RPIN, LOW);
        digitalWrite(LPIN, HIGH);
    }
}

void lcdPrint(const String &text, const int rij, const bool clear)
{
    if (clear)
        lcd.clear();
    lcd.setCursor(rij, 0);
    lcd.print(text);
}

float berekeningFl()
{
    return tan(meetPotRad());
}

float berekeningFd(long tVal)
{
    float hoekRad = meetPotRad();
    return (sin(hoekRad)-2*plankLengte/(9.81*tVal*tVal))/cos(hoekRad);
}

float meetPotRad()
{
    int potWaarde = analogRead(POTPIN);
    float waardeGrad = map(potWaarde, 0, 1023, 0, 360);
    float waardeRad = waardeGrad * (PI / 180);
    return waardeRad;
}