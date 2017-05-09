#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define TIMEOUT 100

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
  radio.maskIRQ(1,1,1);                   // Interrupção desabilitada

  network.begin(/*channel*/ 90, /*node address*/ 00);
}
void loop(void){
  
  network.update();                  // Check the network regularly
    
  
}

void escreve_pino_rede(uint16_t endereco, int pino, boolean valor) {
  RF24NetworkHeader header(endereco, 1);

  struct payload {
    int pino;
    boolean valor;
  } payload = {pino, valor};

  bool ok = network.write(header,&payload,sizeof(payload));
  
  if (ok)
    serial.print("escrito no pino via rede");
  else
    serial.print("falha na escrita de pino pela rede");
}

int le_pino_rede(uint16_t endereco, int pino) {
  RF24NetworkHeader header(endereco, 2);
  boolean leitura;
  unsigned long tempo;

  bool ok = network.write(header,&pino,sizeof(int));
  
  if (!ok) {
    serial.print("falha ao enviar pedido de leitura de pino");
    return -1;
  }
  
  tempo = milis();
  while (!network.available) {
    if (milis() - tempo > TIMEOUT) {
      return -1;
      serial.println("falha ao receber leitura de pino");
    }
  }

  network.read(header, &leitura, sizeof(boolean);
  
  if (leitura == HIGH)
    return 1;
  else
    return 0;
}

void escreve_pwm_pino_rede(uint16_t endereco, int pino, int valor) {
  RF24NetworkHeader header(endereco, 3);

  struct payload {
    int pino;
    int valor;
  } payload = {pino, valor};

  bool ok = network.write(header,&payload,sizeof(payload));
  
  if (ok)
    serial.print("pwm escrito no pino via rede");
  else
    serial.print("falha na escrita de pwn em pino pela rede");
}

int le_pino_analogico_rede(uint16_t endereco, int pino) {
  RF24NetworkHeader header(endereco, 4);
  boolean leitura;
  unsigned long tempo;

  bool ok = network.write(header,&pino,sizeof(int));
  
  if (!ok) {
    serial.print("falha ao enviar pedido de leitura de pino");
    return -1;
  }
  
  tempo = milis();
  while (!network.available) {
    if (milis() - tempo > TIMEOUT) {
      return -1;
      serial.println("falha ao receber leitura de pino");
    }
  }

  network.read(header, &leitura, sizeof(boolean);
  
  if (leitura == HIGH)
    return 1;
  else
    return 0;
}
