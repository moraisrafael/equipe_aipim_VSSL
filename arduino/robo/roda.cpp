#include "roda.h"

Roda::Roda(int dirPinA, int dirPinB, int PWMpin) 
	: pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT){
	
	_dirPinA = dirPinA;
	_dirPinB = dirPinB;
	_PWMpin = PWMpin;
	pid.SetMode(AUTOMATIC);
}

void Roda::movimenta(){
	Input = encoder.rpm();
	pid.Compute();

	if (dir == 1)
	{
		//digitalWrite(_dirPinA, HIGH);
    	//digitalWrite(_dirPinB, LOW);
	} else {
		//digitalWrite(_dirPinA, LOW);
    	//digitalWrite(_dirPinB, HIGH);
	}

	if (_ultimorpm == 0)
	{
		analogWrite(_PWMpin, 255);
		_ultimorpm = Output;
		_ultimopwm = 255;
	} else {
		analogWrite(_PWMpin, (_ultimopwm * Output) / Input);
		_ultimopwm = (_ultimopwm * Output) / Input;
		_ultimorpm = Output;
	}

	Serial.println(_ultimopwm);
}