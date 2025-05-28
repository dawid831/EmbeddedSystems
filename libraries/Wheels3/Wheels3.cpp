#include <Arduino.h>
#include <TimerOne.h>
#include "Wheels3.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

#define RAMPING_TIME 20
#define PER_CM_TIME 25
#define IMPULSES_PER_CM 2

extern volatile unsigned int cntLeft;
extern volatile unsigned int cntRight;
extern volatile long int intPeriod;
extern volatile unsigned int spd;

Wheels3::Wheels3(LiquidCrystal_I2C* display) {
  lcd = display;
}

void Wheels3::setBeepPin(uint8_t pin) {
  _beepPin = pin;
  pinMode(_beepPin, OUTPUT);
}

void Wheels3::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels3::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels3::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels3::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels3::speed(int s) 
{
    spd=s;
    intPeriod = 50000000/s;
}

void Wheels3::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);

}

void Wheels3::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels3::forwardLeft()
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels3::forwardRight()
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels3::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels3::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels3::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels3::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels3::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels3::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels3::stop()
{
    this->stopLeft();
    this->stopRight();
}

/*
* 45 ms ~ 1 cm
* 70 ms ~ 2 cm
* 95 ms ~ 3 cm
*/
void Wheels3::goForward(int cm)
{
    if (cm < 1) return;

    uint8_t targetImpulses = cm * IMPULSES_PER_CM;
    cntLeft = 0;
    cntRight = 0;

    this->forward();
    Serial.println("Starting movement");
    this->setSpeed(spd);

    while (cntLeft < targetImpulses || cntRight < targetImpulses) {
        lcd->setCursor(0, 0);
        lcd->print("L:");
        lcd->print(cntLeft);
        lcd->print(" R:");
        lcd->print(cntRight);
        lcd->print("     ");

        // animacja jazdy przód
        lcd->setCursor(0, 1);
        lcd->print("FWD ");
        lcd->setCursor(6, 1);
        lcd->print(">>>");  // animacja może być np. ">>>" lub obracana

        lcd->setCursor(12, 1);
        lcd->print("FWD");

        delay(100); // aktualizacja co 100ms
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

/*
* 45 ms ~ 1 cm
* 70 ms ~ 2 cm
* 95 ms ~ 3 cm
*/
void Wheels3::goBack(int cm)
{
    if (cm < 1) return;

    uint8_t targetImpulses = cm * IMPULSES_PER_CM;
    cntLeft = 0;
    cntRight = 0;
    Timer1.start(); // Uruchom beep

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
    Timer1.stop(); // Zatrzymaj beep
    digitalWrite(_beepPin, LOW); // upewnij się że wyłączony

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


