#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "protocoloRobo.h"

#define TIMEOUT 10000

RF24 radio(9, 10); // radio ligado nos pinos 9 e 10
RF24Network network(radio);

void verificaRadio();
void trataMensagens();

uint16_t enderecoRobo = 01;
uint16_t enderecoBase = 00;

RadioBuffer radioReadBuffer;

void setup(void) {
  Serial.begin(57600);
  Serial.println("Executando Base");
  Serial.setTimeout(20);

  SPI.begin();
  radio.begin();

  //radio.IRQMask(1,1,0) // interrupcao de recebimento de mensagem ativada
  //attachInterrupt(digitalPinToInterrupt(2), trataMensagens, FALLING);
  //attachInterrupt(0, trataMensagens, FALLING);

  network.begin(/*channel*/ 90, /*node address*/ enderecoBase);

  delay(10);
  RF24NetworkHeader header(enderecoRobo,Ping);
  network.write(header, NULL, 0);
}

  void loop() {
    network.update();
    trataSerial();
    //verificaRadio();
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
    //radioReadBuffer.write(header, msg);
    network.update();
  }
}

void trataSerial() {
  int tipo;
  static RF24NetworkHeader header(enderecoRobo, 0);

  if (Serial.available() > 0) {
    tipo = Serial.parseInt(); // recebe o tipo da mensagem
    Serial.println(tipo);

    switch (tipo) {
    case EscrevePino:
      {
        payloadEscrevePino msg;
        int pino = Serial.parseInt();
        int estado = Serial.parseInt();
        msg.pino = (uint8_t)pino;
        msg.estado = estado == 1;
        header.type = EscrevePino;
        if (!network.write(header, &msg, TAM_MAX_MSG))
          Serial.println("Falha ao escrever no pino");
      }
      break;
    case EscrevePwm:
      {
        payloadEscrevePwm msg;
        int pino = Serial.parseInt();
        int pwm = Serial.parseInt();
        msg.pino = (uint8_t)pino;
        msg.pwm = (uint8_t)pwm;
        header.type = EscrevePwm;
        if (!network.write(header, &msg, TAM_MAX_MSG))
          Serial.println("Falha ao escrever no pino");
      }
      break;
    case MotorDirecaoPwm:
      break;
    case MotorDirecaoVelocidade:
      break;
    case MotorDirecaoDistancia:
      break;
    case EscreveConfiguracao:
      break;
    case Ping:
      {
        header.type = Ping;
        unsigned long time = micros();
        if (!network.write(header, NULL, 0))
          Serial.println("Falha no ping");
        // ACK automatico
        Serial.print("ping = ");
        Serial.println(micros() - time);
      }
      break;
    case Echo:
      {
        char msg[256];
        int size = Serial.readBytesUntil('\0', msg, 256);
        msg[size] = '\0';
        size++;
        header.type = Echo;
        if (!network.write(header, msg, size))
          Serial.println("Falha no echo");
        if (!leRadio(&msg)) {
          Serial.println("Nao recebeu echo");
          break;
        }
        Serial.print("Echo: ");
        Serial.println(msg);
        break;
      }
      break;
    case softwareReset:
      {
        header.type = softwareReset;
        if (!network.write(header, NULL, 0))
          Serial.println("Falha no reset");
      }
      break;
    case LeituraPino:
      {
        payloadLeituraPino msg;
        int pino = Serial.parseInt();
        msg.pino = (uint8_t)pino;
        header.type = LeituraPino;
        if (!network.write(header, &msg, TAM_MAX_MSG))
          Serial.println("Falha ao ler pino");
        if (!leRadio(&msg)) {
          Serial.println("Nao recebeu leitura do pino");
          break;
        }
        Serial.print("pino ");
        Serial.print(pino);
        Serial.print(": ");
        Serial.println(msg.estado);
        break;
      }
      break;
    case LeituraAnalogica:
      {
        payloadLeituraAnalogica msg;
        int pino = Serial.parseInt();
        msg.pino = (uint8_t)pino;
        header.type = LeituraAnalogica;
        if (!network.write(header, &msg, TAM_MAX_MSG))
          Serial.println("Falha ao ler pino");
        if (!leRadio(&msg)) {
          Serial.println("Nao recebeu leitura do pino");
          break;
        }
        Serial.print("pino ");
        Serial.print(pino);
        Serial.print(": ");
        Serial.println(msg.estado);
        break;
      }
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

bool leRadio(void* msg) {
  int time = micros();
  while (micros() - time <= TIMEOUT) {
    network.update();
    if (network.available()) {
      RF24NetworkHeader header;
      if (!network.read(header, &msg, TAM_MAX_MSG)) {
        Serial.println("falha na leitura do radio");
        return false;
      }
      return true;
    }
  }
  Serial.println("TIMEOUT");
  return (false);
}
