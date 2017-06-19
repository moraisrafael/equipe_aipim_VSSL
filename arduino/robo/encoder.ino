#define ENCODER_BUFFER_SIZE 64

class encoder {
	private:
		int ultimo;
		unsigned long ultimo_tempo
	public:
		unsigned long tempo[ENCODER_BUFFER_SIZE];
		int count;
		encoder() {
			// inicializa com o valor maximo no vetor
			count = ultimo = 0;
			for (int i = 0; i < ENCODER_BUFFER_SIZE; i++)
				tempo[i] = 0 - 1;
			ultimo_tempo = micros();
		}
		void update() {
			unsigned long tempo = micros();
			// guarda o tempo entre a ultima medida no encoder ate a atual
			tempo[ultimo] = tempo - ultimo_tempo;
			ultimo_tempo = tempo;
			count++;
			ultimo++;
		}
}
