#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define TIMEOUT 100
#define pino_led 13
#define pino_potenciometro A0

void escreve_pino_rede(uint16_t endereco, int pino, boolean valor);
int le_pino_rede(uint16_t endereco, int pino);
void escreve_pwm_pino_rede(uint16_t endereco, int pino, int valor);
int le_pino_analogico_rede(uint16_t endereco, int pino);
unsigned long ping(uint16_t endereco);

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio

void setup(void)
{
  Serial.begin(9600);
  Serial.setTimeout(10);
  Serial.println("iniciando execucao do echo");
 
  SPI.begin();
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);          // Potência da transmissão em 0dB ( 1mW )
  radio.setDataRate(RF24_250KBPS);        // O padrão é 1Mbps
  radio.setCRCLength(RF24_CRC_16);        // Comprimento do CRC: 8 ou 16 bits
  radio.enableDynamicPayloads();          // Habilita mensagens de tamanho dinâmico
  radio.maskIRQ(1,1,1);                   // Interrupção desabilitada

  network.begin(/*channel*/ 90, /*node address*/ 01);
  
  Serial.setTimeout(100);
}
void loop(void) {
  RF24NetworkHeader header(00, 65);
  network.update();                  // Check the network regularly
  char msg[32];
  //int a = 5;
  Serial.println("loop");
  if(network.available()) {
    network.read(header, &msg, 32);
    Serial.println(msg);
    Serial.println();
    delay(500);
    network.write(header, "Outro teste", 32);
    for(int i= 0; i < 32; i++)
      msg[i] = 0;
  }
}
  
