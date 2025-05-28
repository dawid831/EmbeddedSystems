#include "Wheels3.h"
#include "TimerOne.h"
#include <LiquidCrystal_I2C.h>

// pin, na którym obserwujemy działanie
// pin 13 to dioda LED, ale możesz podłączyć też głośnik
#define BEEPER 13

// wstępny okres w milisekundach
long int intPeriod = 250000;

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

Wheels3 w(&lcd);
volatile char cmd;
volatile unsigned int spd = 100;
volatile unsigned int cntLeft = 0;
volatile unsigned int cntRight = 0;
volatile bool prevLeft = false;
volatile bool prevRight = false;

void setup() {
  w.attach(4, 5, 3 ,7 ,8 ,11);

  lcd.init();
  lcd.backlight();

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
  pinMode(BEEPER, OUTPUT);
  Timer1.initialize();
  TimerUpdate();
  Timer1.stop();
  digitalWrite(BEEPER, LOW);
  w.setBeepPin(13);
  
  Serial.begin(9600);
  Serial.println("Forward: W");
  Serial.println("Stop: S");
  Serial.println("Back: X");
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
      case 'o': spd += 20; w.speed(spd); TimerUpdate(); Timer1.stop(); digitalWrite(BEEPER, LOW); break;
      case 'p': spd -= 20; w.speed(spd); TimerUpdate(); Timer1.stop(); digitalWrite(BEEPER, LOW); break;
      case '1': w.goForward(100); break;
      case '2': w.goBack(100); break;
    }
    Serial.print("Speed: ");
    Serial.println(spd);
  }
}
// POZBYC SIE TIMER1 W KOLEJNEJ LISCIE!
// 3 i 11 do silnikow!

ISR(PCINT1_vect){
  bool currentLeft = (PINC & (1 << PC0));
  bool currentRight = (PINC & (1 << PC1));

  if (currentLeft && !prevLeft) {
    cntLeft++;  // zbocze narastające
  }
  if (currentRight && !prevRight) {
    cntRight++;
  }

  prevLeft = currentLeft;
  prevRight = currentRight;
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
