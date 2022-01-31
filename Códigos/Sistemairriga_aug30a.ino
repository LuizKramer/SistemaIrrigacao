#include "arduino_secrets.h"
// Inclusão das Cibliotecas e Classes
#include <NTPClient.h>
#include "ClassIrrig.h"
#include "thingProperties.h"

// Declaração de variáveis e objetos globais
Valvula v1(0);
Valvula v2(4);
Valvula v3(5);
Valvula v4(14);
Valvula bomb(16);
int horaInicial = 8;
int minutoInicial = 0;

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
  pinMode(v4.getId(), OUTPUT);
  pinMode(bomb.getId(), OUTPUT);
  irrig = false;
  bomba = false;
}

void loop() {
 
  // Atualização dos dados 
  ArduinoCloud.update();
  timeClient.update();
  delay(1000);
  
  // Verificação do horário e execução do processo de irrigação
  if((timeClient.getHours() == horaInicial) && (timeClient.getMinutes() == minutoInicial)){
    processoIrrig(20,30,45);
  }
  
  if(bomba != true){
    systemShutdown();
  }
  
}

void onBombaChange() {
  if(irrig==true && bomba == false){
    systemShutdown();
    irrig = false;
  }
  if(bomba == true){
    bomb.ligar();
  }
  if(bomba == false){
    bomb.desligar();
  }
}

void onIrrigChange() {
  if(irrig == false){
    systemShutdown();
    bomba = false;
  }
  if(irrig == true){
    v1.ligar();
    v2.ligar();
    v3.ligar();
    v4.ligar();
    bomb.ligar();
    bomba = true;
  }
}

void processoIrrig(int tV1, int tV2, int tV3){
      int min;
      v1.ligar();
      v2.ligar();
      v3.ligar();
      bomb.ligar();
      bomba = true;
      while(timeClient.getHours() == horaInicial){
        ArduinoCloud.update();
        timeClient.update();
       
        min = timeClient.getMinutes();
        
        if(min == tV1){
          v3.desligar();
          v4.ligar();
        }
        if(min == tV2){
          v2.desligar();
          v4.desligar();
        }
        if(min == tV3){
          v1.desligar();
        }
        
        if(v1.getStatus() && v2.getStatus() && v3.getStatus() && v4.getStatus()){
          bomb.desligar();
          bomba = false;
          break;
        }
        
      }
      systemShutdown();
}

void systemShutdown(){
    v1.desligar();
    v2.desligar();
    v3.desligar();
    v4.desligar();
    bomb.desligar();
}
