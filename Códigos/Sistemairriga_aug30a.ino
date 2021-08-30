#include "arduino_secrets.h"
// Inclusão das Cibliotecas e Classes
#include <NTPClient.h>
#include "ClassIrrig.h"
#include "thingProperties.h"

// Declaração de variáveis e objetos globais
Valvula v1(0);
Valvula v2(4);
Valvula v3(5);
Valvula bomb(16);
int horaInicial = 8;
int minutoInicial = 30;

// Configuração dos parâmetros para utilizar a biblioteca "NTPCient.h"
const long utcOffsetInSeconds = -10800;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(9600);
  delay(2000);

  //Inicia as propriedades da classe "thingProperties.h"
  initProperties();

  // Conecção com o IoTCloud e inicio da biblioteca "NTPCient.h"
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  timeClient.begin();
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
  
  // Definição I/O das portas e status
  pinMode(A0, INPUT);
  pinMode(v1.getId(), OUTPUT);
  pinMode(v2.getId(), OUTPUT);
  pinMode(v3.getId(), OUTPUT);
  pinMode(bomb.getId(), OUTPUT);
  irrig = true;
  bomba = true;
}

void loop() {
 
  // Atualização dos dados 
  ArduinoCloud.update();
  timeClient.update();
  delay(1000);
  sensor = analogRead(A0);
  
  // Verificação do horário e execução do processo de irrigação
  if((timeClient.getHours() == horaInicial) && (timeClient.getMinutes() == minutoInicial)){
    processoIrrig(15,20,45);
  }
  
}

void onBombaChange() {
  if(irrig==true && bomba == false){
    v1.desligar();
    v2.desligar();
    v3.desligar();
    irrig = false;
  }
  bomb.setStatus();
}

void onIrrigChange() {
    bomba = irrig;
    v1.setStatus();
    v2.setStatus();
    v3.setStatus();
    bomb.setStatus();
}

void processoIrrig(int tV1, int tV2, int tV3){
    int min;
    v1.ligar();
    v2.ligar();
    v3.ligar();
    bomb.ligar();
    bomba = true;
    while(bomba = true){
      ArduinoCloud.update();
      timeClient.update();
     
      if(timeClient.getMinutes() >= horaInicial){
        min = timeClient.getMinutes() - horaInicial;
      }
      else{
        min = timeClient.getMinutes() + horaInicial;
      }
      
      if(min == tV1){
        v1.desligar();
      }
      if(min == tV2){
        v1.desligar();
      }
      if(min == tV3){
        v1.desligar();
      }
      
      if(v1.getStatus() && v2.getStatus() && v3.getStatus()){
        bomb.desligar();
        bomba = false;
      }
      
    }
}

void onSensorChange() {
  // Do something
}
