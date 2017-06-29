#include "roda.h"

Roda::Roda(int dirPinA, int dirPinB, int PWMpin) 
	: pid(&rpmInput, &pwmOutput, &rpmSetpoint, Kp, Ki, Kd, DIRECT){

	pinMode(dirPinA, OUTPUT);
	pinMode(dirPinB, OUTPUT);
	pinMode(PWMpin, OUTPUT);

	this->dirPinA = dirPinA;
	this->dirPinB = dirPinB;
	this->PWMpin = PWMpin;
	pid.SetMode(AUTOMATIC);
	pid.SetSampleTime(0);
	pid.SetOutputLimits(0,255);
	encoder.reset();
}

void Roda::movimenta(){

	rpmInput = encoder.rpm();
	pid.Compute();
	analogWrite(PWMpin, pwmOutput);
}


void Roda::setTunings(double kp, double ki, double kd) {
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;
	pid.SetTunings(kp, ki, kd);
}

void Roda::setDir(int dir) {

	if (dir == 1)
	{
    	digitalWrite(dirPinB, LOW);
		digitalWrite(dirPinA, HIGH);
	} else {
		digitalWrite(dirPinA, LOW);
    	digitalWrite(dirPinB, HIGH);
	}
}
