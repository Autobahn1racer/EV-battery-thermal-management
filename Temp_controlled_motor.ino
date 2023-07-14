#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;


const int A1A = 6;
const int A1B = 7;
const int B1A = 4;
const int B2A = 5;


void setup() {
  // Initialize serial and wait for port to open:
  pinMode(A1A,OUTPUT);
  pinMode(A1B,OUTPUT);
  pinMode(B1A,OUTPUT);
  pinMode(B2A,OUTPUT);

  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
 
  // Defined in thingProperties.h
  initProperties();
 
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //Get Cloud Info/errors , 0 (only errors) up to 4
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
 
  //Wait to get cloud connection to init the carrier
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }
  delay(500);
  CARRIER_CASE = true;
  carrier.begin();
  carrier.display.setRotation(0);
   
  delay(1500);
}
 
void loop() {
  ArduinoCloud.update();
  carrier.Buttons.update();
 
  while(!carrier.Light.colorAvailable()) {
    delay(5);
  }
  int none;
  carrier.Light.readColor(none, none, none, light);
  
  temperature = carrier.Env.readTemperature()-9;
 
  if (carrier.Buttons.onTouchDown(TOUCH0)) {
    carrier.display.fillScreen(ST77XX_WHITE);
    carrier.display.setTextColor(ST77XX_RED);
    carrier.display.setTextSize(2);
 
    carrier.display.setCursor(30, 110);
    carrier.display.print("Temp: ");
    carrier.display.print(temperature);
    carrier.display.print(" C");
  }
 
  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    carrier.display.fillScreen(ST77XX_WHITE);
    carrier.display.setTextColor(ST77XX_RED);
    carrier.display.setTextSize(2);
 
    carrier.display.setCursor(30, 110);
    carrier.display.print("Light: ");
    carrier.display.print(light);
  }
  
  if (light < 10){
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,HIGH);
    digitalWrite(B1A,LOW);
    digitalWrite(B2A,HIGH);
    Serial.print("ON");
  }
  else{
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,LOW);
    digitalWrite(B1A,LOW);
    digitalWrite(B2A,LOW);
  }
  
  if (light < 10){
    weather_report = "Motor On";
  }
  else{
    weather_report = "Motor Off";
  }
 
}
