#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define MAX_PAYLOAD_SIZE2 10
#define pino_led 8
#define pino_potenciometro A0

typedef struct payload_escreve_pino {
  int pino;
  boolean valor;
} payload_escreve_pino;

typedef struct payload_escreve_pino_pwm {
  int pino;
  int valor;
} payload_escreve_pino_pwm;

void trata_msg();
void escreve_pino(byte* payload);
void le_pino(byte* payload);
void escreve_pino_pwm(byte* payload);
void le_pino_analogico(byte* payload);

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio

void setup(void)
{
  Serial.begin(57600);
  Serial.println("iniciando execucao da base");
 
  SPI.begin();
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);          // Potência da transmissão em 0dB ( 1mW )
  radio.setDataRate(RF24_250KBPS);        // O padrão é 1Mbps
  radio.setCRCLength(RF24_CRC_16);        // Comprimento do CRC: 8 ou 16 bits
  radio.enableDynamicPayloads();          // Habilita mensagens de tamanho dinâmico
  radio.maskIRQ(1,1,0);                   // Interrupção apenas para mensagens recebidas

  network.begin(/*channel*/ 90, /*node address*/ 01);

  // Habilita interrupção no pino 2 do Arduino
  attachInterrupt(digitalPinToInterrupt(2), trata_msg, FALLING);

  pinMode(pino_led, OUTPUT);
  pinMode(pino_potenciometro, INPUT);
}
void loop(void){
  network.update();                  // Check the network regularly  
  // mensagens recebidas sao tratadas com interrupcoes
}

void trata_msg() {
  RF24NetworkHeader header;
  byte payload[MAX_PAYLOAD_SIZE2];

  while (network.available()) {
    network.read(header, payload, MAX_PAYLOAD_SIZE2);
    
    if (header.type == 1)
      escreve_pino(payload);
    else if (header.type == 2)
      le_pino(payload);
    else if (header.type == 3)
      escreve_pino_pwm(payload);
    else if (header.type == 4)
      le_pino_analogico(payload);
    else
      Serial.println("tipo nao reconhecido");
  }
}

void escreve_pino(byte* payload) {
  digitalWrite(((payload_escreve_pino*) payload)->pino, ((payload_escreve_pino*) payload)->valor);
}

void le_pino(byte* payload) {
  RF24NetworkHeader header(00, 2);
  boolean leitura;

  leitura = digitalRead((int)*payload);

  network.write(header,&leitura,sizeof(boolean));
}

void escreve_pino_pwm(byte* payload) {

  analogWrite(((payload_escreve_pino_pwm*)payload)->pino, ((payload_escreve_pino_pwm*)payload)->valor);
}

void le_pino_analogico(byte* payload) {
  RF24NetworkHeader header(00, 4);
  int leitura;

  leitura = analogRead((int)*payload);

  network.write(header,&leitura,sizeof(int));
}
