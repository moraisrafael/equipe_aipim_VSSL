#ifndef encoder_h
#define encoder_h
#define ENCODER_BUFFER_SIZE 64

#include "Arduino.h"
class Encoder
{
  public:
  	unsigned long tempo[ENCODER_BUFFER_SIZE];
	int count;    
    Encoder();
    void update();
    int rpm();
  private:
    int ultimo;
	unsigned long ultimo_tempo;
};
#endif
