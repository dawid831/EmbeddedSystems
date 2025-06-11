#include "Wheels4.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3

// pin kontroli serwo (musi być PWM)
#define SERVO 3

Servo serwo;

byte LCDAddress = 0x27;

LiquidCrystal_I2C lcd(LCDAddress, 16, 2);

Wheels4 w(&lcd);
volatile char cmd;
volatile unsigned int spd = 200;
volatile unsigned int cntLeft = 0;
volatile unsigned int cntRight = 0;
volatile bool prevLeft = false;
volatile bool prevRight = false;

void setup() {
    Serial.begin(9600);
    w.attach(7, 8, 5 ,10 ,9 ,6);
    Serial.println("Attached wheels");

    lcd.init();
    lcd.backlight();
    Serial.println("LCD ready");

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
    pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
    pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

    serwo.attach(SERVO);

    /* patrz przed siebie */
    serwo.write(90);

    Serial.println("Forward: W");
    Serial.println("Stop: S");
    Serial.println("Back: X");
}

void loop() { 
    w.goForward();
    while(Serial.available())
    {
        cmd = Serial.read();
        switch(cmd)
        {
            case 'w': w.forward(); break;
            case 'x': w.back(); break;
            case 's': w.stop(); break;
            case 'o': spd += 20; w.speed(spd); break;
            case 'p': spd -= 20; w.speed(spd); break;
            case '1': w.goForward(); break;
            case '2': w.goBack(100); break;
            default : w.goForward(); break;
        }
        Serial.print("Speed: ");
        Serial.println(spd);
    }
}


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
