#include <Arduino.h>
#include "Wheels6.h"


#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)

Wheels6::Wheels6(LiquidCrystal_I2C* display) {
  lcd = display;
}

void Wheels6::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels6::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels6::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
}

void Wheels6::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
}

void Wheels6::setSpeed(uint8_t s)
{
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels6::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels6::forwardLeft()
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
}

void Wheels6::forwardRight()
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
}

void Wheels6::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
}

void Wheels6::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
}

void Wheels6::forward()
{
    this->forwardLeft();
    this->forwardRight();
}

void Wheels6::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels6::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels6::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels6::stop()
{
    this->stopLeft();
    this->stopRight();
}


void Wheels6::goForward()
{
    this->forward();
    this->setSpeed(200);

    lcd->setCursor(0, 0);
    lcd->print("VROOM VROOM VROOM");
    lcd->setCursor(0, 1);
    lcd->print("FWD ");
    lcd->setCursor(6, 1);
    lcd->print(">>>");
    lcd->setCursor(12, 1);
    lcd->print("FWD");
}


void Wheels6::goBack()
{
    this->back();
    this->setSpeed(200);

    lcd->setCursor(0, 0);
    lcd->print("VROOM VROOM VROOM");
    lcd->setCursor(0, 1);
    lcd->print("BCK ");
    lcd->setCursor(6, 1);
    lcd->print(">>>");
    lcd->setCursor(12, 1);
    lcd->print("BCK");
}

void Wheels6::turnLeft()
{
    this->forwardLeft();
    this->backRight();
    this->setSpeed(150);

    lcd->setCursor(0, 0);
    lcd->print("RGHT TURN RGHT  ");
    lcd->setCursor(0, 1);
    lcd->print("LEFT ");
    lcd->setCursor(6, 1);
    lcd->print("<<<");
    lcd->setCursor(12, 1);
    lcd->print("LEFT");
}

void Wheels6::turnRight()
{
    this->backLeft();
    this->forwardRight();
    this->setSpeed(150);

    lcd->setCursor(0, 0);
    lcd->print("LEFT TURN LEFT ");

    lcd->setCursor(0, 1);
    lcd->print("RGHT ");
    lcd->setCursor(6, 1);
    lcd->print(">>>");
    lcd->setCursor(12, 1);
    lcd->print("RGHT");
}

