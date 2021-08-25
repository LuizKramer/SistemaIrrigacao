#include "arduino_secrets.h"
// NTPClient - Version: Latest
#include <NTPClient.h>


#include "thingProperties.h"

#define bombaPort  16
#define V1  5
#define V2 4
#define V3  0
#define CXD  12

bool on = false;
bool off = true;



const long utcOffsetInSeconds = -10800;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(2000);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  timeClient.begin();
  pinMode(A0, INPUT);
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();
  
  pinMode(bombaPort, OUTPUT);
  pinMode(V1, OUTPUT);
  pinMode(V2, OUTPUT);
  pinMode(V3, OUTPUT);
  pinMode(CXD, OUTPUT);
  
  digitalWrite(bombaPort, HIGH);
  digitalWrite(V1, HIGH);
  digitalWrite(V2, HIGH);
  digitalWrite(V3, HIGH);
  digitalWrite(CXD, HIGH);
  irrig = off;
  bomba = off;
  
}

void loop() {
  ArduinoCloud.update();
  //Serial.println(analogRead(A0));
  timeClient.update();
  //Serial.print(timeClient.getFormattedTime());
  delay(1000);
  sensor = analogRead(A0);
  
  if((timeClient.getHours() == 8) && (timeClient.getMinutes() == 30)){
   bomba = irrig;
    progressoIrrig=0;
    digitalWrite(bombaPort, on);
    digitalWrite(V1, on);
    digitalWrite(V2, on);
    digitalWrite(V3, on);
    delay(900000);
    progressoIrrig=12.5;
    digitalWrite(V1, off);
    delay(600000);
    progressoIrrig=37.5;
    digitalWrite(V2, off);
    delay(900000);
    progressoIrrig=100;
    digitalWrite(V3, off);
    digitalWrite(bombaPort, off);
    irrig=false;
    bomba= false;
  }
}

void onBombaChange() {
  if(irrig==true && bomba == false){
    
    digitalWrite(V1, true);
    digitalWrite(V2, true);
    digitalWrite(V3, true);
    irrig = false;
    
  }
  
  Serial.print("Bomba:");
  Serial.println(bomba);
  digitalWrite(bombaPort, !bomba);
  
}

void onIrrigChange() {
    bomba = irrig;
    digitalWrite(bombaPort, !irrig);
    digitalWrite(V1, !irrig);
    digitalWrite(V2, !irrig);
    digitalWrite(V3, !irrig);
    
    
}

void onProgressoIrrigChange() {
}

void onSensorChange() {
  // Do something
}
