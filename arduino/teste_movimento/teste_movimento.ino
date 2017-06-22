#include <PID_v1.h>
#include "encoder.h"
#include "pins.h"
#include "roda.h"

Roda RodaEsquerda(MTR_AIN1, MTR_AIN2, MTR_PWMA);
Roda RodaDireita(MTR_BIN2, MTR_BIN1, MTR_PWMB);

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

  RodaEsquerda.Setpoint = 15;
  RodaDireita.Setpoint = 15;

}

void loop() {
	if (Serial.available()) {
		char c;
		double kp, ki, kd;

		noInterrupts();
		c = Serial.read();
		
		switch (c) {
			case ('r'):
				Serial.print("ultimo rpm = ");
				Serial.println(RodaEsquerda.pwmSetpoint);
				Serial.print("mudar para rpm =");
				RodaDireita.Serial.pwmSetpoint = RodaEsquerda.Serial.pwmSetpoint = Serial.paseFloat();
				Serial.println(RodaEsquerda.pwmSetpoint);
				break;
			case ('p'):
				Serial.print("ultima configuracao do PID: kp=");
				Serial.print(RodaEsquerda.kp);
				Serial.print(" ki=");
				Serial.print(RodaEsquerda.ki);
				Serial.print(" kd=");
				Serial.println(RodaEsquerda.kd);
				Serial.print("mudar para: kp=");
				kp = Serial.paseFloat();
				Serial.print(kp);
				Serial.print(" ki=");
				ki = Serial.paseFloat();
				Serial.print(ki);
				Serial.print(" kd=");
				kd = Serial.paseFloat();
				Serial.println(kd);
				RodaEsquerda.SetTunings(kp,ki,kd):
				RodaDireita.SetTunings(kp,ki,kd):
				break
			default:
				Serial.println("r - mudar rpm");
				Serial.println("p - mudar configuracao do PID);
		}
		Interrupts();
	}
}

void updateEsquerda(){
  RodaEsquerda.encoder.update();
  RodaEsquerda.movimenta();
  
  Serial.print("esquerda tempo:");
  Serial.print(RodaEsquerda.encoder.ultimoTempo);
  Serial.print("int:");
  Serial.print(RodaEsquerda.encoder.ultimoIntervalo);
  Serial.print("rpm:");
  Serial.print(RodaEsquerda.encoder.rpm());
  Serial.print("pwm:");
  Serial.print(RodaEsquerda.pwmOutput);
}
void updateDireita(){
  RodaDireita.encoder.update();
  RodaDireita.movimenta();

  Serial.print("direita tempo:");
  Serial.print(RodaEsquerda.encoder.ultimoTempo);
  Serial.print("int:");
  Serial.print(RodaEsquerda.encoder.ultimoIntervalo);
  Serial.print("rpm:");
  Serial.print(RodaEsquerda.encoder.rpm());
  Serial.print("pwm:");
  Serial.print(RodaEsquerda.pwmOutput);
}
