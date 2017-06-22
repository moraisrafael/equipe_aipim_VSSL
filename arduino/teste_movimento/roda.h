#ifndef roda_h
#define roda_h
#define ENCODER_BUFFER_SIZE 64
#define Kp 0.2
#define Ki 0.5
#define Kd 0.1

#include "Arduino.h"
#include "encoder.h"
#include <PID_v1.h>



class Roda
{
  public:
  	Encoder encoder;
  	int dir;
  	double rpmSetpoint, rpmInput, pwmOutput;
	double kp, ki, kd;
  	PID pid;
    Roda(int dirPinA, int dirPinB, int PWMpin);
    void movimenta();
    void setTunings(float kp, ki, kd);
    void setDir(int dir);
  private:
  	int dirPinA, dirPinB, PWMpin;
};
#endif
