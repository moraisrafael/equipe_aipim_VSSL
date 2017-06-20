#include <PID_v1.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "protocoloRobo.h"
#include "encoder.h"
#include "pins.h"
#include "roda.h"

Roda RodaEsquerda(MTR_AIN1, MTR_AIN2, MTR_PWMA);
Roda RodaDireita(MTR_BIN1, MTR_BIN2, MTR_PWMB);

RF24 radio(9, 10); // radio ligado nos pinos 9 e 10
RF24Network network(radio);

void verificaRadio(); // colocar com interrupcao por tempo
void trataMensagens();
void movimentaRobo();

uint16_t enderecoRobo = 01;
uint16_t enderecoBase = 00;

RadioBuffer radioReadBuffer;

void setup(void) {
  Serial.begin(57600);
  Serial.println("Executando Robo");
  
  pinMode(IRQ_ENC_A, INPUT);
  pinMode(IRQ_ENC_B, INPUT);

  attachInterrupt(IRQ_ENC_A, updateEsquerda, FALLING);
  attachInterrupt(IRQ_ENC_B, updateDireita, FALLING);

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ enderecoRobo);
}

void loop() {
  verificaRadio();
  trataMensagens();
  movimentaRobo();
}

void updateEsquerda(){
  RodaEsquerda.encoder.update();
}
void updateDireita(){
  RodaDireita.encoder.update();
}

void verificaRadio() {
  static RF24NetworkHeader header;
  static byte msg[TAM_MAX_MSG];
  network.update(); // necessario para network funcionar corretamente
  while (network.available())
  {
    if (radioReadBuffer.isFull())
      return;
    network.read(header, msg, TAM_MAX_MSG);
    radioReadBuffer.write(header, msg);
    network.update();
  }
}

//typedef int payloadEscrevePino;

void trataMensagens() {
  static uint8_t tipo;
  byte msg[TAM_MAX_MSG];
  static RF24NetworkHeader header(enderecoBase, 0);

  while (radioReadBuffer.available()) {
    radioReadBuffer.read(tipo, msg);
    switch (tipo) {
    case EscrevePino:
      {
        int pino = ((payloadEscrevePino*)msg)->pino;
        bool estado = ((payloadEscrevePino*)msg)->estado;
        if (estado == 0)
          digitalWrite(pino,LOW);
        else
          digitalWrite(pino,HIGH);
      }
      break;
    case EscrevePwm:
      analogWrite(((payloadEscrevePwm*)msg)->pino, ((payloadEscrevePwm*)msg)->pwm);
      break;
    case MotorDirecaoPwm:
      break;
    case MotorDirecaoVelocidade:
      RodaDireita.dir = ((payloadLeituraVelocidade*)msg)->direcao;
      RodaEsquerda.dir = ((payloadLeituraVelocidade*)msg)->direcao;
      RodaDireita.Setpoint = ((payloadLeituraVelocidade*)msg)->velocidadeDireita;
      RodaEsquerda.Setpoint = ((payloadLeituraVelocidade*)msg)->velocidadeEsquerda;
      break;
    case MotorDirecaoDistancia:
      break;
    case EscreveConfiguracao:
      break;
    case Ping:
      // ACK automatico
      break;
    case Echo:
      header.type = Echo;
      network.write(header, msg, TAM_MAX_MSG);
      break;
    case softwareReset:
      // reset();
      break;
    case LeituraPino:
      header.type = LeituraPino;
      ((payloadLeituraPino*)msg)->estado = digitalRead(((payloadLeituraPino*)msg)->pino);
      network.write(header, msg, sizeof(payloadLeituraPino));
      break;
    case LeituraAnalogica:
      header.type = LeituraPino;
      ((payloadLeituraAnalogica*)msg)->estado = analogRead(((payloadLeituraAnalogica*)msg)->pino);
      network.write(header, msg, sizeof(payloadLeituraPino));
      break;
    case LeituraVelocidade:
      break;
    case LeituraEncoder:
      break;
    case LeituraConfiguracao:
      break;
    default:
      // mensgaem nao reconhecida
      break;
    }
  }
}

void movimentaRobo() {
  RodaDireita.movimenta();
  RodaEsquerda.movimenta();
}

