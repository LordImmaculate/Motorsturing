// Motorsturing
//

#include <Arduino.h>

#define PWMPIN 3
#define LPIN 4
#define RPIN 5

#define LASERDICHT 13
#define LASERVER 12
#define MICROSWITCHONDER 11
#define MICROSWITCHBOVEN 10
#define STARTKNOP 9
#define RESETKNOP 8

void setup()
{ 
  Serial.begin(9600);
  pinMode(PWMPIN, OUTPUT);
  pinMode(LPIN, OUTPUT);
  pinMode(RPIN, OUTPUT);
  
  pinMode(LASERDICHT, INPUT);
  pinMode(LASERVER, INPUT);
  pinMode(MICROSWITCHONDER, INPUT);
  pinMode(MICROSWITCHBOVEN, INPUT);
  pinMode(STARTKNOP, INPUT);
  pinMode(RESETKNOP, INPUT);
}

void loop()
{
  
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