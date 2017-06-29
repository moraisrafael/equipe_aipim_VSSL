#include <PID_v1.h>
#include "encoder.h"
#include "pins.h"
#include "roda.h"

Roda RodaEsquerda(MTR_AIN1, MTR_AIN2, MTR_PWMA);
Roda RodaDireita(MTR_BIN2, MTR_BIN1, MTR_PWMB);

void updateEsquerda();
void updateDireita();

void setup(void) {
  Serial.begin(57600);
  Serial.println("Executando teste de movimento");
  
  pinMode(IRQ_ENC_A, INPUT);
  pinMode(IRQ_ENC_B, INPUT);
  
  attachInterrupt(0, updateEsquerda, FALLING);
  attachInterrupt(1, updateDireita, FALLING);

  digitalWrite(MTR_BIN1, LOW);
  digitalWrite(MTR_AIN1, HIGH);
  digitalWrite(MTR_AIN2, LOW);
  digitalWrite(MTR_BIN2, HIGH);

  RodaEsquerda.rpmSetpoint = 30;
  RodaDireita.rpmSetpoint = 30;

}

unsigned long ultimo_tempo = 0;

void loop() {
        if (millis() - ultimo_tempo >= 10) {
          RodaDireita.movimenta();
          RodaEsquerda.movimenta();
        }
        ultimo_tempo = millis();

	if (Serial.available()) {
		char c;
		double kp, ki, kd;

		noInterrupts();
		c = Serial.read();
		
		switch (c) {
			case ('r'):
				Serial.print("ultimo rpm = ");
				Serial.println(RodaEsquerda.rpmSetpoint);
				Serial.print("mudar para rpm =");
				RodaDireita.rpmSetpoint = RodaEsquerda.rpmSetpoint = Serial.parseFloat();
				Serial.println(RodaEsquerda.rpmSetpoint);
				break;
			case ('p'):
				Serial.print("ultima configuracao do PID: kp=");
				Serial.print(RodaEsquerda.kp);
				Serial.print(" ki=");
				Serial.print(RodaEsquerda.ki);
				Serial.print(" kd=");
				Serial.println(RodaEsquerda.kd);
				Serial.print("mudar para: kp=");
				kp = Serial.parseFloat();
				Serial.print(kp);
				Serial.print(" ki=");
				ki = Serial.parseFloat();
				Serial.print(ki);
				Serial.print(" kd=");
				kd = Serial.parseFloat();
				Serial.println(kd);
				RodaEsquerda.setTunings(kp,ki,kd);
				RodaDireita.setTunings(kp,ki,kd);
				break;
			default:
				Serial.println("r - mudar rpm");
				Serial.println("p - mudar configuracao do PID");
		}
		interrupts();
	}
}

void updateEsquerda(){
  RodaEsquerda.encoder.update();
  RodaEsquerda.movimenta();
  
  Serial.print("esquerda tempo:");
  Serial.println(RodaEsquerda.encoder.ultimoTempo);
  /*Serial.print("int:");
  Serial.print(RodaEsquerda.encoder.ultimoIntervalo);
  Serial.print("rpm:");
  Serial.print(RodaEsquerda.encoder.rpm());
  Serial.print("pwm:");
  Serial.println(RodaEsquerda.pwmOutput);*/
}
void updateDireita(){
  RodaDireita.encoder.update();
  noInterrupts();
  RodaDireita.movimenta();
  interrupts();

  Serial.print("direita tempo:");
  Serial.println(RodaEsquerda.encoder.ultimoTempo);
  /*Serial.print("int:");
  Serial.print(RodaEsquerda.encoder.ultimoIntervalo);
  Serial.print("rpm:");
  Serial.print(RodaEsquerda.encoder.rpm());
  Serial.print("pwm:");
  Serial.println(RodaEsquerda.pwmOutput);*/
}
