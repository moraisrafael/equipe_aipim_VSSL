#ifndef encoder_h
#define encoder_h
#define ENCODER_BUFFER_SIZE 64

#include "Arduino.h"
class Encoder
{
public:
  int count;
  unsigned long ultimoTempo, ultimoIntervalo;
  void update();
  float rpm();
  Encoder();
  void reset();
};
#endif

