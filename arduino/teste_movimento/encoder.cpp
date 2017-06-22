#include "Arduino.h"
#include "encoder.h"

void Encoder::update() {
	unsigned long tempoAtual;

	noInterrupts();
	tempoAtual = millis();
	ultimoIntervalo = tempoAtual - ultimoTempo;
	ultimoTempo = tempoAtual;
	count++;
	interrupts();
}

float Encoder::rpm() {
	
  return 60000000.0 / (ulimoIntervalo * 48.0); //um minuto em us dividido pelo tempo que a roda leva pra dar uma volta em us
}

Encoder::Encoder() {
	ultimoTempo = micros();
	ultimoIntervalo = 0;
	count = 0;
}
