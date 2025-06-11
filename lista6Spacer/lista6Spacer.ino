#include "Wheels5Spacer.h"
#include <Servo.h>

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3

// pin kontroli serwo (musi być PWM)
#define SERVO 3

Servo serwo;

Wheels5Spacer w;
volatile char cmd;
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

    Serial.println("Spacer!");
    w.goForward();
}

void loop() 
{ }
