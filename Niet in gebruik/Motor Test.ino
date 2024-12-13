// Motorsturing
//

#include <Arduino.h>

#define PWMPIN 3
#define LPIN 4
#define RPIN 5

void setup()
{ 
  Serial.begin(9600);
  pinMode(PWMPIN, OUTPUT);
  pinMode(LPIN, OUTPUT);
  pinMode(RPIN, OUTPUT);
}

void loop()
{
  digitalWrite(PWMPIN, HIGH);
  motorSturen(true);
  delay(5000);
  motorSturen(false);
  delay(5000);
}

void motorSturen(bool richting)
{
  if(richting)
  {
    digitalWrite(LPIN, LOW);
    digitalWrite(RPIN, HIGH);
    return;
  }
  
  else
  {
    digitalWrite(RPIN, LOW);
    digitalWrite(LPIN, HIGH);
    return;
  }
}