#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio

void setup(void)
{
  Serial.begin(9600);
  Serial.println("iniciando execucao do echo");
 
  SPI.begin();
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);          // Potência da transmissão em 0dB ( 1mW )
  radio.setDataRate(RF24_250KBPS);        // O padrão é 1Mbps
  radio.setCRCLength(RF24_CRC_16);        // Comprimento do CRC: 8 ou 16 bits
  radio.enableDynamicPayloads();          // Habilita mensagens de tamanho dinâmico
  radio.maskIRQ(1,1,1);                   // Interrupção desabilitada

  network.begin(/*channel*/ 90, /*node address*/ 01);
}
void loop(void) {
  RF24NetworkHeader header;
  char msg[256];

  network.update();                  // Check the network regularly
  Serial.println("loop");
  if(network.available()) {
    network.read(header, msg, 256);
    Serial.println(msg);
    Serial.println();
    //delay(500);
    header.to_node = header.from_node;
    header.from_node = 01;
    header.type = 65;
    network.write(header, msg, 256);
  }
}
