#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define TIMEOUT 100
#define pino_led 8
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
}
void loop(void){
  network.update();                  // Check the network regularly

  Serial.println("loop");
  while (Serial.available() == 0);
  int test = Serial.parseInt();
  Serial.println(test);
  switch (test) {
    case 1 :// testa escrita digital
      {
      for (int i = 0; i < 2; i++) {
        escreve_pino_rede(01, pino_led, HIGH);
        delay(500);
        escreve_pino_rede(01, pino_led, LOW);
        delay(500);
      }
      break;
      }
    case 2 : // testa leitura digital
      {
      boolean val = le_pino_rede(01, pino_potenciometro);
      if (val == HIGH)
        Serial.println("HIGH");
      else
        Serial.println("LOW");
      break;
      }
    case 3 : // testa escrita pwm
      {
      int brightness = 0;
      for (int i = 0; i < 1024; i++) {
        escreve_pwm_pino_rede(01, pino_led, brightness);
        brightness = (brightness + 5)%256;
        delay(30);
      }
      break;
      }
    case 4 : // testa leitura analogica
      {
      unsigned long time = millis();
      while (millis() - time < 5000) {
        Serial.println(le_pino_analogico_rede(01, pino_potenciometro));
      }
      break;
      }
    case 65 : // testa perda de pacotes
      {
      int count = 0;
      unsigned long latencia, latencia_media = 0;
      Serial.println("iniciando teste de perda de pacotes");
      for (int i = 0; i < 1000; i++) {
        Serial.print(i);
        latencia = ping(01);
        if (latencia == 0)
          count++;
        latencia_media += latencia;
      }
      latencia_media = latencia_media/(1000 - count);
      Serial.print("pacotes perdidos: ");
      Serial.println(count);
      Serial.print("latencia media: ");
      Serial.println(latencia_media);
      break;
      }
    case -1 : // teste leitura analogica e escrita pwm
      {
      unsigned long time = millis();
      while (millis() - time < 5000) {
        int valor = le_pino_analogico_rede(01, pino_potenciometro);
        escreve_pwm_pino_rede(01, pino_led, valor);
      }
      break;
      }
    case -2 :  // teste 65, com 3 radios ao mesmo tempo
      {
      int count1, count2, count3;
      unsigned long latencia, latencia_media1, latencia_media2, latencia_media3;
      count1 = count2 = count3 = 0;
      latencia_media1 = latencia_media2 = latencia_media3 = 0;
      for (int i = 0; i < 1000; i++) {
        latencia = ping(01);
        if (latencia == 0)
          count1++;
        latencia_media1 += latencia;
  
        latencia = ping(02);
        if (latencia == 0)
          count2++;
        latencia_media2 += latencia;
  
        latencia = ping(03);
        if (latencia == 0)
          count3++;
        latencia_media3 += latencia;
      }
      latencia_media1 = latencia_media1/(1000 - count1);
      Serial.print("pacotes perdidos: ");
      Serial.println(count1);
      Serial.print("latencia media: ");
      Serial.println(latencia_media1);
      latencia_media2 = latencia_media2/(1000 - count2);
      Serial.print("pacotes perdidos: ");
      Serial.println(count2);
      Serial.print("latencia media: ");
      Serial.println(latencia_media2);
      latencia_media3 = latencia_media3/(1000 - count3);
      Serial.print("pacotes perdidos: ");
      Serial.println(count3);
      Serial.print("latencia media: ");
      Serial.println(latencia_media3);
      break;
      }
    default:
      Serial.println("comando invalido");
  }
}

void escreve_pino_rede(uint16_t endereco, int pino, boolean valor) {
  RF24NetworkHeader header(endereco, 1);

  struct payload {
    int pino;
    boolean valor;
  } payload = {pino, valor};

  network.write(header,&payload,sizeof(payload));
}

int le_pino_rede(uint16_t endereco, int pino) {
  RF24NetworkHeader header(endereco, 2);
  boolean leitura;
  unsigned long tempo;

  network.write(header,&pino,sizeof(int));
  
  tempo = millis();
  while (!network.available()) {
    if (millis() - tempo > TIMEOUT) {
      return -1;
      Serial.println("falha ao receber leitura de pino");
    }
  }

  network.read(header, &leitura, sizeof(boolean));
  
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

  network.write(header,&payload,sizeof(payload));
}

int le_pino_analogico_rede(uint16_t endereco, int pino) {
  RF24NetworkHeader header(endereco, 4);
  boolean leitura;
  unsigned long tempo;

  network.write(header,&pino,sizeof(int));

  tempo = millis();
  while (!network.available()) {
    if (millis() - tempo > TIMEOUT) {
      return -1;
      Serial.println("falha ao receber leitura de pino");
    }
  }

  network.read(header, &leitura, sizeof(boolean));
  
  if (leitura == HIGH)
    return 1;
  else
    return 0;
}

unsigned long ping(uint16_t endereco) {
  RF24NetworkHeader header(endereco, 65);
  unsigned long time;
  bool ack;
  Serial.print(" ping ");
  time = micros();
  ack = network.write(header,NULL,0);
  if (ack == true) {
    Serial.println(micros() - time);
    return micros() - time;
  } else {
    Serial.println("falhou");
    return 0;
  }
}
