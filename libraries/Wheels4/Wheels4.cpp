#include <Arduino.h>
#include <TimerOne.h>
#include <Servo.h>
#include "Wheels4.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

#define RAMPING_TIME 20
#define PER_CM_TIME 25
#define IMPULSES_PER_CM 2
// piny dla sonaru (HC-SR04)
#define TRIG A2
#define ECHO A3
// pin kontroli serwo (musi być PWM)
#define SERVO 3

extern volatile unsigned int cntLeft;
extern volatile unsigned int cntRight;
extern volatile unsigned int spd;
extern volatile Servo serwo;

Wheels4::Wheels4(LiquidCrystal_I2C* display) {
  lcd = display;
}

void Wheels4::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels4::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels4::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels4::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels4::speed(int s) 
{
    spd=s;
}

void Wheels4::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);

}

void Wheels4::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels4::forwardLeft()
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels4::forwardRight()
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels4::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels4::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels4::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels4::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels4::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels4::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels4::stop()
{
    this->stopLeft();
    this->stopRight();
}

int Wheels4::sonarDistance(byte angle) {
    unsigned long tot;      // czas powrotu (time-of-travel)
    unsigned int distance;

    serwo.write(angle);
    delay(200);

    /* uruchamia sonar (puls 10 ms na `TRIGGER')
    * oczekuje na powrotny sygnał i aktualizuje
    */
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    tot = pulseIn(ECHO, HIGH);

    /* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
    * droga tam i z powrotem, zatem:
    */
    distance = tot/58;
    return distance;
}

int Wheels4::minDistForward() {
    int min = 100;
    for (int i = 60; i < 130; i += 60) {
        int dist = sonarDistance(i);
        if (dist < min) min = dist;
    }
    return min;
}

bool Wheels4::findObstacleFree() {
    this->setSpeed(0);
    this->forwardLeft();
    this->backRight();
    int dist = minDistForward();
    if (dist > 20) return true;
    while (dist <= 20) {
        lcd->setCursor(0, 0);
        lcd->print("dist:");
        lcd->print(dist);
        lcd->print(" angle:");
        lcd->print(90);
        lcd->print("     ");
        dist = minDistForward();
        Serial.println("Seeking path");
        this->setSpeed(140);
    }
    Serial.println("Path open");
    return true;
}

/*
* Jedz dopoki nie natrafisz na przeszkode.
*/
void Wheels4::goForward()
{
    cntLeft = 0;
    cntRight = 0;

    int dist = minDistForward();

    this->forward();
    Serial.println("Starting movement");
    if (dist > 20) this->setSpeed(spd);

    while (dist > 5) {
        lcd->setCursor(0, 0);
        lcd->print("dist:");
        lcd->print(dist);
        lcd->print(" angle:");
        lcd->print(90);
        lcd->print("     ");

        if (dist > 30) {
            if (dist < 50) {
                this->setSpeed(140);
            } else {
                this->setSpeed(200);
            }
            // animacja jazdy przód
            lcd->setCursor(0, 1);
            lcd->print("FWD ");
            lcd->setCursor(6, 1);
            lcd->print(">>>");  // animacja może być np. ">>>" lub obracana

            lcd->setCursor(12, 1);
            lcd->print("FWD");
        } else if (findObstacleFree()) {
            this->forward();
        } else {
            break;
        }
        delay(100); // aktualizacja co 100ms
        dist = minDistForward();
    }

    this->stop();
    Serial.println("Movement stopped");

    lcd->setCursor(0, 0);
    lcd->print("dist:");
    lcd->print(dist);
    lcd->print(" angle:");
    lcd->print(90);
    lcd->print("     ");

    lcd->setCursor(6, 1);
    lcd->print("   ");
    lcd->setCursor(0, 1);
    lcd->print("STP");
    lcd->setCursor(6, 1);
    lcd->print("XXX");
    lcd->setCursor(12, 1);
    lcd->print("STP");
}

/*
* 45 ms ~ 1 cm
* 70 ms ~ 2 cm
* 95 ms ~ 3 cm
*/
void Wheels4::goBack(int cm)
{
    if (cm < 1) return;

    uint8_t targetImpulses = cm * IMPULSES_PER_CM;
    cntLeft = 0;
    cntRight = 0;

    this->back();
    Serial.println("Starting movement");
    this->setSpeed(spd);

    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        lcd->setCursor(0, 0);
        lcd->print("L:");
        lcd->print(cntLeft);
        lcd->print(" R:");
        lcd->print(cntRight);
        lcd->print("     ");

        // animacja jazdy tył
        lcd->setCursor(0, 1);
        lcd->print("BCK");
        lcd->setCursor(6, 1);
        lcd->print("<<<");
        lcd->setCursor(12, 1);
        lcd->print("BCK");

        delay(100);
    }

    this->stop();
    Serial.println("Movement stopped");

    lcd->setCursor(0, 0);
    lcd->print("L:");
    lcd->print(cntLeft);
    lcd->print(" R:");
    lcd->print(cntRight);
    lcd->print("     ");

    lcd->setCursor(6, 1);
    lcd->print("   ");
    lcd->setCursor(0, 1);
    lcd->print("STP");
    lcd->setCursor(6, 1);
    lcd->print("XXX");
    lcd->setCursor(12, 1);
    lcd->print("STP");
}


