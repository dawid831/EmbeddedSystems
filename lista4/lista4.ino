#include "Wheels3.h"
#include "TimerOne.h"
#include <LiquidCrystal_I2C.h>

// pin, na którym obserwujemy działanie
// pin 13 to dioda LED, ale możesz podłączyć też głośnik
#define BEEPER 13

// wstępny okres w milisekundach
long int intPeriod = 500000;

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

Wheels3 w(&lcd);
volatile char cmd;

void setup() {
  w.attach(4, 5, 3 ,7 ,8 ,9);

  lcd.init();
  lcd.backlight();

  pinMode(BEEPER, OUTPUT);
  Timer1.initialize();
  TimerUpdate();
  Timer1.stop();
  w.setBeepPin(13);
  
  Serial.begin(9600);
  Serial.println("Forward: WAD");
  Serial.println("Back: ZXC");
  Serial.println("Stop: S");
}

void loop() {
  while(Serial.available())
  {
    cmd = Serial.read();
    switch(cmd)
    {
      case 'w': w.forward(); break;
      case 'x': w.back(); break;
      case 's': w.stop(); break;
      case '1': w.setSpeedLeft(75); break;
      case '2': w.setSpeedLeft(200); break;
      case '3': w.setSpeedRight(75); break;
      case '4': w.setSpeedRight(200); break;
      case '5': w.setSpeed(100); break;
      case 'l': w.goForward(15); break;
      case 'k': w.goBack(20); break;
    }
  }
}

// aktualizuje Timer1 aktualną wartością intPeriod
void TimerUpdate() {
  Timer1.detachInterrupt();
  Timer1.attachInterrupt(doBeep, intPeriod);
}

// zmienia wartość pinu BEEPER
void doBeep() {
  digitalWrite(BEEPER, digitalRead(BEEPER) ^ 1);
}
