#include "Wheels5.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3

// pin kontroli serwo (musi być PWM)
#define SERVO 3

Servo serwo;

Wheels5 w;
volatile unsigned int spd = 200;

void setup() {
    Serial.begin(9600);
    w.attach(7, 8, 5 ,10 ,9 ,6);
    Serial.println("Attached wheels");

    pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
    pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls

    serwo.attach(SERVO);

    /* patrz przed siebie */
    serwo.write(90);

    Serial.println("Sprezyna");
    w.spring();
}

void loop() {}
