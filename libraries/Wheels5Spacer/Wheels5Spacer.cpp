#include <Arduino.h>
#include <Servo.h>
#include "Wheels5Spacer.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3
// pin kontroli serwo (musi być PWM)
#define SERVO 3

extern volatile unsigned int spd;
extern volatile Servo serwo;

Wheels5Spacer::Wheels5Spacer() 
{ }

void Wheels5Spacer::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels5Spacer::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels5Spacer::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels5Spacer::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels5Spacer::speed(int s) 
{
    spd=s;
}

void Wheels5Spacer::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);

}

void Wheels5Spacer::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels5Spacer::forwardLeft()
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels5Spacer::forwardRight()
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels5Spacer::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels5Spacer::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels5Spacer::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels5Spacer::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels5Spacer::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels5Spacer::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels5Spacer::stop()
{
    this->stopLeft();
    this->stopRight();
}

#define NUM_SAMPLES 5

int Wheels5Spacer::sonarDistance(byte angle) {
    unsigned long tot;
    unsigned int distances[NUM_SAMPLES]; 

    serwo.write(angle);
    delay(200);

    for (int i = 0; i < NUM_SAMPLES; i++) {
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);
        
        tot = pulseIn(ECHO, HIGH, 30000);
        
        distances[i] = (tot == 0) ? 200 : (tot / 58);
        delay(100); // minimalny odstęp między pomiarami
    }
    
    // Filtracja - średnia
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += distances[i];
    }
    return sum / NUM_SAMPLES;
}

void Wheels5Spacer::goForward()
{
    // 1. etap - wyjazd z sali
    Serial.println("1.");
    int dist = sonarDistance(180);
    delay(300);

    this->forward();
    this->setSpeed(200);

    Serial.println(dist);
    while (dist < 100) {
        dist = sonarDistance(180);
        delay(100);
        Serial.println(dist);
    }
    this->stop();
    
    delay(100);
    // 2. etap - zakręt w lewo
    Serial.println("2.");
    dist = sonarDistance(180);
    delay(300);

    this->backLeft();
    this->forwardRight();
    this->setSpeed(150);
    delay(1200);
    this->stop();

    // 3. etap - jazda prosto do końca korytarza
    Serial.println("3.");
    this->forward();
    this->setSpeed(160);
    delay(2000);
    dist = 100;
    int distLeft = sonarDistance(180);
    delay(200);

    while(dist > 80) {
        if (distLeft < 40) {
            this->setSpeedLeft(220);
            this->setSpeedRight(120);
        } else{
            this->setSpeedLeft(120);
            this->setSpeedRight(220);
        }

        dist = sonarDistance(80);
        delay(200);
        distLeft = sonarDistance(180);
    }
    this->stop();

    // 4. etap - zawrócenie
    Serial.println("4.");
    this->backLeft();
    this->forwardRight();
    this->setSpeed(150);
    delay(2700);
    this->stop();

    // 5. etap - jazda z powrotem
    Serial.println("5.");
    this->forward();
    this->setSpeed(160);
    delay(2000);
    distLeft = sonarDistance(0);
    delay(200);

    while(distLeft < 180) {
        if (distLeft < 35) {
            this->setSpeedLeft(120);
            this->setSpeedRight(220);
        } else{
            this->setSpeedLeft(220);
            this->setSpeedRight(120);
        }

        distLeft = sonarDistance(0);
        delay(200);
    }
    this->stop();

    // 6. etap - wjazd do sali
    Serial.println("6.");
    this->forwardLeft();
    this->backRight();
    this->setSpeed(150);
    delay(1200);
    this->stop();

    this->forward();
    this->setSpeed(150);
}
