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
  Serial.println("iniciando execucao da base");
 
  SPI.begin();
  radio.begin();

  radio.setPALevel(RF24_PA_MAX);          // Potência da transmissão em 0dB ( 1mW )
  radio.setDataRate(RF24_250KBPS);        // O padrão é 1Mbps
  radio.setCRCLength(RF24_CRC_16);        // Comprimento do CRC: 8 ou 16 bits
  radio.enableDynamicPayloads();          // Habilita mensagens de tamanho dinâmico
  radio.maskIRQ(1,1,1);                   // Interrupção desabilitada

  network.begin(/*channel*/ 90, /*node address*/ 00);
  
  Serial.setTimeout(100);
  randomSeed(666);
  testaCodigo();
}
void loop(void) {
  /*
  RF24NetworkHeader header(01, 65);
  network.update();                  // Check the network regularly
  char msg[32], msg2[32];
  int tam;

  //tam = random()%257;
  tam = 32;
  for (int i = 0; i < tam -1; i++)
     msg[i] = random()%23 + 'a';
  msg[31] = '\0';
  Serial.print("vou enviar: ");
  Serial.println(msg);
  network.write(header, &msg, 32);
  network.read(header, &msg2, 32);
  Serial.print("recebi:     ");
  Serial.println(msg2);
    
    //for (int i = 0; i < tam; i++) {
      //if (msg[i] != msg2[i]) {
        //Serial.println("deu zica");
        //break;
      //}
    //}
    String str = msg;
    String str2 = msg2;
    Serial.println(header.type);
    if (str == str2)
      Serial.println("sucesso");
    else
      Serial.println("falhou");
    delay(1000);
  */
}

void testaCodigo() {
  RF24NetworkHeader header(01, 65);
  char msg[32];
  network.write(header, "Teste", 32);
  delay(500);
  network.read(header, &msg, 32);
  
  String str = "Teste";
  String str2 = msg;
  
  if (str == str2) {
    Serial.println("Ae");
  }
  Serial.println(str);
  Serial.println(str2);

} 
