#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define MAX_PAYLOAD_SIZE 10

void trata_msg();
void escreve_pino_rede(uint16_t endereco, int pino, boolean valor);
boolean le_pino_rede(uint16_t endereco, int pino);
void escreve_pwm_pino_rede(uint16_t endereco, int pino, int valor);
int le_pino_analogico_rede(uint16_t endereco, int pino);

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
  attachInterrupt(0, trata_msg, FALLING);
}
void loop(void){
  
  network.update();                  // Check the network regularly
    
  
}

void_trata_msg() {
  RF24NetworkHeader header;
  byte[MAX_PAYLOAD_SIZE] payload;

  while (network.available()) {
    network.read(header, payload, MAX_PAYLOAD_SIZE);
    
    if (header.type == 1)
      escreve_pino(payload);
    else if (header.type == 2)
      le_pino(payload);
    else if (header.type == 3)
      escreve_pino_pwm(payload);
    else if (header.type == 4)
      le_pino_analogico(payload);
    else
      serial.println("tipo nao reconhecido");
  }
}

void escreve_pino(byte* payload) {

  struct payload {
    int pino;
    boolean valor;
  };

  digitalWrite((struct payload*) payload->pino, (struct payload*) payload->valor);
}

void le_pino(byte* payload) {
  RF24NetworkHeader header(00, 2);
  boolean leitura;

  leitura = digitalRead((int)*payload);

  bool ok = network.write(header,&leitura,sizeof(boolean));
  
  if (!ok) {
    serial.print("falha ao enviar leitura de pino");
    return -1;
  }
}

void escreve_pino_pwm(byte* payload) {
  struct payload {
    int pino;
    int valor;
  };

  analogWrite((struct payload*) payload->pino, (struct payload*) payload->valor);
}

void le_pino_analogico(uint16_t endereco, int pino) {
  RF24NetworkHeader header(00, 4);
  int leitura;

  leitura = analogRead((int)*payload);

  bool ok = network.write(header,&leitura,sizeof(int));
  
  if (!ok) {
    serial.print("falha ao enviar leitura pwm de pino");
    return -1;
  }
}
