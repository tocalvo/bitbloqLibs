/*
 * BitbloqFreaksCar.cpp
 *
 * Copyright 2017 Alberto Valero <alberto.valero@bq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "Arduino.h"
#include "BitbloqFreaksCar.h"



BitbloqFreaksCar::BitbloqFreaksCar():
        usSensor(NULL),
        leftDCMotor(NULL),
        rightDCMotor(NULL),
        buzzerPin(11),
        usTriggerPin(8),
        usEchoPin(7),
        DCMotor1Dir(5),
        DCMotor1PWM(6),
        DCMotor2Dir(9),
        DCMotor2PWM(10),
        endStopPin(4),
        ldrRightPin(A5),
        ldrLeftPin(A6),
        irPinArray{A4,A3,A2,A1,A0}
{
	usSensor = new BitbloqUltrasound(usTriggerPin,usEchoPin);
    leftDCMotor = new BitbloqDCMotor(DCMotor2Dir,DCMotor2PWM);
    rightDCMotor = new BitbloqDCMotor(DCMotor1Dir,DCMotor1PWM);
}


BitbloqFreaksCar::~BitbloqFreaksCar(){
	
	//check that all of them are not NULL pointers, delete and set to NULL
	
    if(usSensor!=NULL){
        delete usSensor;
        usSensor=NULL;
    }
    
    if(leftDCMotor!=NULL){
        delete leftDCMotor;
        leftDCMotor=NULL;
    }
    
    if(rightDCMotor!=NULL){
        delete rightDCMotor;
        rightDCMotor=NULL;
    }
}

void BitbloqFreaksCar::setup(){
	//IR Pins
	for (int i = 0; i<5; i++){
		pinMode(irPinArray[i],INPUT);
	}
	
	//LDR PINS
	pinMode(ldrRightPin,INPUT);
	pinMode(ldrLeftPin,INPUT);
	
	//Buzzer
	pinMode(buzzerPin,OUTPUT);

	//US Sensor setup
	usSensor->setup();
        
    //dc motors setup
    leftDCMotor->setup();
    rightDCMotor->setup();
    
    //endstop
    pinMode(endStopPin,INPUT);

}


int BitbloqFreaksCar::readUSMeasuredDistanceCM() const{
	return (usSensor != NULL ? usSensor->readDistanceInCM(): - 1 ); //in centimeters
}

int BitbloqFreaksCar::readUSMeasuredDistanceIN() const{
	return (usSensor != NULL ? usSensor->readDistanceInInches() : -1); //in inches
}

int BitbloqFreaksCar::readLDRRight() const{
	return analogRead(ldrRightPin);
}

int BitbloqFreaksCar::readLDRLeft() const{
	return analogRead(ldrLeftPin);
}
   
void BitbloqFreaksCar::move(int direction, int speed){
	int leftSpeed = 0;
	int rightSpeed = 0;
	if(direction == 1){
		leftSpeed = speed; //forward 
		rightSpeed = -speed;
		
	}else if(direction == 2){//backward
		leftSpeed = -speed;
		rightSpeed = speed;
		
	}else if(direction == 3){
		leftSpeed = -speed; //right
		rightSpeed = -speed;
	}else if(direction == 4){
		
		leftSpeed = speed; //left
		rightSpeed = speed;
	}
	
    leftDCMotor->setSpeed(leftSpeed);
    rightDCMotor->setSpeed(rightSpeed);
}

void BitbloqFreaksCar::setRightMotorSpeed(int speed){
    rightDCMotor->setSpeed(speed);
}

void BitbloqFreaksCar::setLeftMotorSpeed(int speed){
    leftDCMotor->setSpeed(speed);
}

byte BitbloqFreaksCar::readIR(int index) const{
	if (index >=0 && index <4)
		return (analogRead(irPinArray[index]) < 650 ? 0 : 1);
	else
		return -1;
}

void BitbloqFreaksCar::playTone(int note, int beat) const{
	tone(buzzerPin, note, beat);
}