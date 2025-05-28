#include "Wheels2.h"
#include <LiquidCrystal_I2C.h>

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

Wheels2 w(&lcd);
volatile char cmd;

void setup() {
  w.attach(4, 5, 3 ,7 ,8 ,9);

  lcd.init();
  lcd.backlight();
  
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
      case '1': w.goForward(19); break;
      case '2': w.goBack(19); break;
    }
  }
}
