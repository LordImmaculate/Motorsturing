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

// Zorgt ervoor dat "Klik op RESET" maar 1 keer om de 5 seconden wordt geprint
unsigned long resetTijd;

void setup()
{
    // Start de seriële monitor en zet de pinmode van alle pins
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
    // Controleren dat de plank op de startpositie staat, anders vragen om te resetten
    if (digitalRead(SWITCH1PIN))
    {
        displayOnLcd("Druk op START");

        // Wachten tot de startknop wordt ingedrukt
        resetTijd = 0;
        while (!start);
        displayOnLcd("START gedrukt");
        traagOmhoog();
        while (start && !sensor1 && !switch2);
        stop();

        if (start && !switch2)
        {
            unsigned long tStart = millis();
            while (!sensor2);
            unsigned long tStop = millis();
            unsigned int tVal = tStop - tStart;
            displayOnLcd("Fl: " + String(berekeningFl()));
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Fd: " + String(berekeningFd(tVal)));
            delay(5000);
        }
    }

    // Zorgen dat "Klik op RESET" maar om de 5 seconden wordt geprint
    else if (millis() - resetTijd > 5000)
    {
        resetTijd = millis();
        displayOnLcd("Klik op RESET");
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

//Functie die automatisch de LCD eerst leegmaakt, zorgt ddt de cursor op de juist positie staat en dan de tekst print
void displayOnLcd(const String &text)
{
    lcd.clear();
    lcd.setCursor(0, 0);
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

// Meet de potentiometer en geeft de radialen terug
float meetPotRad()
{
    int potWaarde = analogRead(POTPIN);
    float waardeGrad = map(potWaarde, 0, 1023, 0, 360);
    float waardeRad = waardeGrad * (PI / 180);
    return waardeRad;
}