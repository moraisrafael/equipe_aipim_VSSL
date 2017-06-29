#include "Arduino.h"
#include "encoder.h"

void Encoder::update() {
  unsigned long tempoAtual;

  noInterrupts();
  //tempoAtual = micros();
  ultimoIntervalo = micros() - ultimoTempo;
  ultimoTempo = micros();
  count++;
  interrupts();
  Serial.println(ultimo_tempo);
}

float Encoder::rpm() {

  return 60000000.0 / (ultimoIntervalo * 48.0); //um minuto em us dividido pelo tempo que a roda leva pra dar uma volta em us
}

void Encoder::reset() {
  ultimoTempo = micros();
  ultimoIntervalo = 0;
  count = 0;
}

Encoder::Encoder() {
  reset();
}

