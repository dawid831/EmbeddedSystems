#include <Arduino.h>
#include <TimerOne.h>
#include <Servo.h>
#include "Wheels5.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3
// pin kontroli serwo (musi być PWM)
#define SERVO 3

extern volatile unsigned int spd;
extern volatile Servo serwo;

Wheels5::Wheels5() 
{ }

void Wheels5::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels5::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels5::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels5::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels5::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
    spd=s;
}

void Wheels5::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels5::forwardLeft()
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels5::forwardRight()
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels5::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels5::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels5::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels5::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels5::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels5::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels5::stop()
{
    this->stopLeft();
    this->stopRight();
}

#define NUM_SAMPLES 5

int Wheels5::sonarDistance() {
    unsigned long tot;
    unsigned int distances[NUM_SAMPLES];    
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        
        tot = pulseIn(ECHO, HIGH, 30000);
        
        if (tot == 0) {
            distances[i] = 999; // wartość dla timeout
        } else {
            distances[i] = tot / 58;
        }
        delay(30); // minimalny odstęp między pomiarami
    }
    
    // Filtracja - średnia
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += distances[i];
    }
    return sum / NUM_SAMPLES;
}


void Wheels5::spring()
{
    int dist = sonarDistance();

    Serial.println("Starting movement");
    if (dist < 95) {
        this->back();
        this->setSpeed(120);
    } else if (dist > 105) {
        this->forward();
        this->setSpeed(120);
    }

    while (true) {
        Serial.println(dist);

        if (dist > 105) {
            this->forward();
            this->setSpeed(120 + (dist-105));

        } else if (dist < 95) {
            this->back();
            this->setSpeed(120 + (100-dist));

        } else {
            this->stop();
            this->setSpeed(0);

        }
        delay(200);
        dist = sonarDistance();
    }
}
