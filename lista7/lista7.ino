#include <IRremote.hpp>
#include <LiquidCrystal_I2C.h>
#include "Wheels6.h"

#define IR_RECEIVE_PIN 2

/* Kody dla pilota:
* 1 - BA45FF00 69
* 2 - B946FF00 70
* 3 - B847FF00 71
* 4 - BB44FF00 68
* 5 - BF40FF00 64
* 6 - BC43FF00 67
* 7 - F807FF00 7
* 8 - EA15FF00 21
* 9 - F609FF00 9
* * - E916FF00 22
* 0 - E619FF00 25
* # - F20DFF00 13
* up - E718FF00 24
* down - AD52FF00 82
* left - F708FF00 8
* right - A55AFF00 90
* OK - E31CFF00 28
*/

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

Wheels6 w(&lcd); 

bool pinAccepted = false;
const uint8_t correctPin[] = {67, 9, 25, 28}; // PIN: 6-9-0-OK
uint8_t pinIndex = 0;

void setup() {
  w.attach(8, 7, 6, 11, 12, 5);

  lcd.init();
  lcd.backlight();
  
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Gotowe do odczytu kodów pilota");
  lcd.setCursor(0, 0);
  lcd.print("Wpisz PIN:");
  lcd.setCursor(0, 1);
  lcd.print("____");
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      IrReceiver.resume();
      return;  // pomiń ramkę powtarzania
    }

    if (IrReceiver.decodedIRData.protocol == NEC) {
      uint8_t cmd = IrReceiver.decodedIRData.command;
      Serial.print("Pilot: ");
      Serial.println(cmd);
      if (!pinAccepted) {
        // Sprawdzanie PIN-u
        if (cmd == correctPin[pinIndex]) {
          lcd.setCursor(pinIndex, 1);
          lcd.print("*");
          pinIndex++;
          if (pinIndex == 4) {
            pinAccepted = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("PIN OK");
            delay(1000);
            lcd.clear();
          }
        } else {
          lcd.setCursor(0, 0);
          lcd.print("PIN NIEPOPRAWNY");
          lcd.setCursor(0, 1);
          lcd.print("____");
          pinIndex = 0;
          delay(1000);
          lcd.setCursor(0, 0);
          lcd.print("Wpisz PIN:");
        }
      } else {
        switch(cmd) {
          case 25: w.stop(); break;
          case 69: w.goForward(); break;
          case 70: w.goBack(); break;
          case 71: w.turnLeft(); break;
          case 68: w.turnRight(); break;
        }
      }
    } else {
      Serial.println("Nieznany protokół lub zakłócenie");
    }

    IrReceiver.resume();
  }

  delay(50);
}
