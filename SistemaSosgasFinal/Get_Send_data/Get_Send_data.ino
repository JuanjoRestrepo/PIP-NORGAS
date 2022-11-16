#include <SoftwareSerial.h>
#include "Ubidots.h"
#include "HX711.h"

#define calibration_factor -15100.00 //This value is obtained using the SparkFun_HX711_Calibration sketch
//16300
#define DOUT  1
#define CLK  2

//WIFI 
const char* UBIDOTS_TOKEN = "BBFF-yIcfddlljpqD1j1PfkljATj9sF8bJh";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "HUAWEI Y9 PRIME";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "edwin123";      // Put here your Wi-Fi password 

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
HX711 scale;

void setup() {
  Serial.begin(9600);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  Serial.println("HX711 scale demo");

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  Serial.print("Reading: ");
  float peso = scale.get_units()*0.45359237;
  Serial.print(peso, 1); //scale.get_units() returns a float
  Serial.print(" Kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  ubidots.add("PESO", peso);// Change for your variable name 
  //ubidots.add("Porcentaje", porcentaje);
  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
  if (bufferSent) {
  // Do something if values were sent properly
   Serial.println("Values sent by the device");
  }
  delay(2000);
}
