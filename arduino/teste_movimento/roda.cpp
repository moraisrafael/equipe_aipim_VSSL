#include "roda.h"

Roda::Roda(int dirPinA, int dirPinB, int PWMpin) 
	: pid(&rpmInput, &pwmOutput, &rpmSetpoint, Kp, Ki, Kd, DIRECT){

	pinMode(dirPinA, OUTPUT);
	pinMode(dirPinB, OUTPUT);
	pinMode(PWMpin, OUTPUT);

	this.dirPinA = dirPinA;
	this.dirPinB = dirPinB;
	this.PWMpin = PWMpin;
	pid.SetMode(AUTOMATIC);
	pid.SetSapleTime(0);
	pid.SetOutputLimits(0,255);
	encoder.Encoder()
}

void Roda::movimenta(){

	rpmInput = encoder.rpm();
	pid.Compute();
	analogWrite(_PWMpin, pwmOutput);
}


void Roda::setTunings(float kp, ki, kd) {
	this.kp = kp;
	this.ki = ki;
	this.kd = kd;
	pid.setTunings(kp, ki, kd)
}

void Roda::setDir(int dir) {

	if (dir == 1)
	{
    	digitalWrite(_dirPinB, LOW);
		digitalWrite(_dirPinA, HIGH);
	} else {
		digitalWrite(_dirPinA, LOW);
    	digitalWrite(_dirPinB, HIGH);
	}
}
