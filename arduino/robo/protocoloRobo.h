#include <RF24Network.h>

#define TAM_BUFFER 8
#define TAM_MAX_MSG 16

class RadioBuffer {
  private:
    byte buffer[TAM_BUFFER][1+TAM_MAX_MSG];
    uint8_t cabeca;
    uint8_t rabo;
    uint8_t nMsgs;
  public:
    RadioBuffer() {
      cabeca = rabo = nMsgs = 0;
    }
    bool write(RF24NetworkHeader header, byte* msg) {
      if (nMsgs == TAM_BUFFER)
        return false;
      buffer[cabeca][0] = header.type;
      for (int i = 0; i < TAM_MAX_MSG; i++)
        buffer[cabeca][i+1] = msg[i];
      cabeca = (cabeca + 1)%TAM_BUFFER;
      nMsgs++;
      return true;
    }
    bool read(uint8_t &type, byte* msg) {
      if (nMsgs == 0)
        return false;
      type = buffer[rabo][0];
      for (int i = 0; i < TAM_MAX_MSG; i++)
        msg[i] = buffer[rabo][i+1];
      rabo = (rabo + 1)%TAM_BUFFER;
      nMsgs--;
      return true;
    }
    bool isFull() {
      return nMsgs == TAM_BUFFER;
    }
    bool isEmpty() {
      return nMsgs == 0;
    }
    bool available() {
      return nMsgs > 0;
    }
};

typedef struct payloadEscrevePino {
  uint8_t pino;
  bool estado;
} payloadEscrevePino;

typedef struct payloadEscrevePwm {
  uint8_t pino;
  uint8_t pwm;
} payloadEscrevePwm;

typedef struct payloadMotorDirecaoPwm {
  uint8_t direcao;
  uint8_t pwmDireita;
  uint8_t pwmEsquerda;
} payloadMotorDirecaoPwm;

typedef struct payloadMotorDirecaoVelocidade {
  uint8_t direcao;
  float velocidadeDireita;
  float velocidadeEsquerda;
} payloadMotorDirecaoVelocidade;

typedef struct payloadMotorDirecaoDistancia {
  uint8_t direcao;
  uint16_t distanciaDireita;
  uint16_t distanciaEsquerda;
} payloadMotorDirecaoDistancia;

typedef struct payloadEscreveConfiguracao {
  // a definir
} payloadEscreveConfiguracao;

typedef struct payloadLeituraPino {
  uint8_t pino;
  bool estado;
} payloadLeituraPino;

typedef struct payloadLeituraAnalogica {
  uint8_t pino;
  uint16_t estado;
} PayloadLeituraAnalogica;

typedef struct payloadLeituraVelocidade {
  uint8_t direcao;
  float velocidadeDireita;
  float velocidadeEsquerda;
} payloadLeituraVelocidade;

typedef struct payloadLeituraEncoder {
  uint16_t direita;
  uint16_t esquerda;
} payloadLeituraEncoder;

typedef struct payloadLeituraConfiguracao {
  // a definir
} payloadLeituraConfiguracao;

typedef enum tipoMensagem {
  EscrevePino, EscrevePwm, MotorDirecaoPwm, MotorDirecaoVelocidade, MotorDirecaoDistancia, EscreveConfiguracao, Ping, Echo, softwareReset, LeituraPino, LeituraAnalogica, LeituraVelocidade, LeituraEncoder, LeituraConfiguracao
} tipomensagem;
