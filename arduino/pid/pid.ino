#define LED         6      // Led conetado a uma saída digital
#define RADIO_CE    8      // Pino CE do módulo de rádio
#define RADIO_CS    7      // Pino CS do módulo do rádio
#define RADIO_A0   A4      // Bit 0 do end. do rádio (LOW = ligado)
#define RADIO_A1   A5      // Bit 1 do end. do rádio (LOW = ligado)
#define IRQ_ENC_A   2      // Pino de interrupção do Encoder A
#define IRQ_ENC_B   3      // Pino de interrupção do Encoder B
#define IRQ_RADIO   4      // Pino de interrupção do Rádio
#define MTR_AIN1   A0      // Bit 0 - Controle da ponte H do Motor A
#define MTR_AIN2   A1      // Bit 1 - Controle da ponte H do Motor A
#define MTR_BIN1   A2      // Bit 0 - Controle da ponte H do Motor B
#define MTR_BIN2   A3      // Bit 1 - Controle da ponte H do Motor B
#define MTR_PWMA    9      // Sinal de PWM para controle  do Motor A
#define MTR_PWMB   10      // Sinal de PWM para controle  do Motor B
#define VOLT_BAT   A6      // Tensão da bateria -> Vcc/10

unsigned long posD=0, posE = 0;
unsigned long lastposD=0, lastposE = 0;
unsigned long tD=0, tE = 0, lasttD = 0,lasttE = 0;

int currentD, currentE;
int finalD = 48;
int finalE = 48;

float Kp =   .9;                                // PID proportional control Gain
float Kd =    .1;
float ki = kp/20;
int pwmD, pwmE;

void setup(){
  Serial.begin(115600);
  Serial.println("Begin");
  
  pinMode(IRQ_ENC_A, INPUT);
  pinMode(IRQ_ENC_B, INPUT);

  pinMode(MTR_AIN1, OUTPUT);
  pinMode(MTR_AIN2, OUTPUT);
  pinMode(MTR_BIN1, OUTPUT);
  pinMode(MTR_BIN2, OUTPUT);
  pinMode(MTR_PWMA, OUTPUT);
  pinMode(MTR_PWMB, OUTPUT);
  
  digitalWrite(MTR_AIN1, HIGH);
  digitalWrite(MTR_AIN2, LOW);
  digitalWrite(MTR_BIN1, LOW);
  digitalWrite(MTR_BIN2, HIGH);
  
  attachInterrupt(1, updateEsquerda, FALLING);
  attachInterrupt(0, updateDireita, FALLING);
  
  analogWrite(MTR_PWMA, 0);
  analogWrite(MTR_PWMB, 0);

}

void updateEsquerda (){
  if (tE != millis()){
    posE++;
    tE = millis();
  }
}

void updateDireita (){
  if (tD != millis()){
    posD++;
    tD = millis();
  }
}


void loop(){
  Serial.print("D: ");
  Serial.println(posD);
  Serial.print("E: ");
  Serial.println(posE);
  
  currentD = posD - lastposD;
  currentE = posE - lastposE;
  lastposD = posD;
  lastposE = posE;
  
  Serial.print("vD: ");
  Serial.println( currentD );
  Serial.print("vE: ");
  Serial.println(currentE);
  
  delay(100);
  pwmD = updatePid(pwmD, 5*48/10, currentD);
  pwmE = updatePid(pwmE, 5*48/10, currentE);  
  
  Serial.print("pwmD: ");
  Serial.println(pwmD);
  Serial.print("pwmE: ");
  Serial.println(pwmE);
  
  analogWrite(MTR_PWMA, pwmD);
  analogWrite(MTR_PWMB, pwmE);
}

int updatePid(int command, int targetValue, int currentValue)   {             // compute PWM value
float pidTerm = 0;                                                            // PID correction
int error=0;                                  
static int last_error=0;
static int error_history[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static int last_error_history = 0;
static int error_history_sum = 0;
int current_error_pos;


 error = abs(targetValue) - abs(currentValue);
 
 current_error_pos = (last_error_history + 1)%20; // posicao a ser substituida
 error_history_sum += error - error_history_sum[current_error_pos]; // subtrai valor mais antigo e soma atual
 error_history[current_error_pos] = error; // guarda erro atual no vetor;
 last_error_history = current_error_history; // atualiza a posicao do ultimo erro recebido
 
 pidTerm = (Kp * error) + (Kd * (error - last_error)) + (ki * (error_history_sum));                            
 last_error = error;
 return constrain(command + int(pidTerm), 0, 255);
}
