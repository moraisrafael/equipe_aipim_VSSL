codigos fonte usados no arduino para controle do rádio e dos robôs.

robo        código a ser usado pelos robôs
base        código para comunicação do computador com os robôs via rádio

Para uso do rádio nRF24l01, estamos usando as bibliotecas:
<a href =https://github.com/nRF24/RF24>RF24</a> (14/maio/2017)
<a href=https://github.com/nRF24/RF24Network>RF24Network</a> (15/abril/2017)

radio funcionando com os comandos:

EscrevePino:         1 <pino> <estado>
EscrevePwm:          2 <pino> <pwm>
Ping:                7
Echo:                8 <mensagem>
LeituraPino:         10 <pino>
LeituraAnalogica:    11 <pino>

para enviar um comando da base para o robo, digite-o no formato acima na serial do arduino base
