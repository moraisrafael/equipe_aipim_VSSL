#include "Arduino.h"
#include "encoder.h"

Encoder::Encoder() {
	// inicializa com o valor maximo no vetor
	count = ultimo = 0;
	for (int i = 0; i < ENCODER_BUFFER_SIZE; i++)
		tempo[i] = 0 - 1;
	ultimo_tempo = micros();
}

void Encoder::update() {
	unsigned long tempoAtual = micros();
	// guarda o tempo entre a ultima medida no encoder ate a atual
	tempo[ultimo] = tempoAtual - ultimo_tempo;
	ultimo_tempo = tempoAtual;
	count++;
	ultimo=  (ultimo + 1) % ENCODER_BUFFER_SIZE;
}

int Encoder::rpm() {
	
  return 60000 / (tempo[ultimo-1] * 48); //um minuto em ms dividido pelo tempo que a roda leva pra dar uma volta em ms
}
