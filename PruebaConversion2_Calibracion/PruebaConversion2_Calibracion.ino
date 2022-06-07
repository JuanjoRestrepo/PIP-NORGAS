#include <SoftwareSerial.h> //Incluimos la libreria Software Serial
SoftwareSerial SIM800L(8,9); //Instanciamos el objeto SIM800L y le pasamos los parametros de los pines TX y RX
#include "HX711.h"  // Libreria de galgas
#define DEBUG_HX711
#define fact_calibracion 31100.0  // Factor de calibración pesa inicial


// Galgas
byte pinData = 6; // Datos analógicos que toma la galga
byte pinClk = 5;  // Configuración de time para toma de datos

// Leds
int LEDV = 10; // Led Verde (100 %- 61 %)
int LEDA = 11; // Led Azul ( 60 % - 31 % )
int LEDR = 12; // Led Rojo ( < 30 % )
int Button = 7; // Botón de Reinicio báscula
int estado = 0;
int relacion =1;
float ref = 40.0;
float porcentaje = 0;
float avg_size = 10.0; // Promedio de la toma de medidas
String TEXTO;
HX711 bascula;


void setup(){
  pinMode (LEDV, OUTPUT);
  pinMode (LEDA, OUTPUT);
  pinMode (LEDR, OUTPUT);
  pinMode (8, OUTPUT); // TX
  pinMode (9, INPUT); // TR
  //pinMode(13, OUTPUT); //Declarar el pin 13 como salida.
  pinMode (Button,INPUT_PULLUP);
 

  /*SIM800L.begin(9600); //Inicializamos la segunda comunicacion Serial.
  SIM800L.println("AT+CMGF=1"); //Vamos utilizar los SMS.
  delay(100); //Delay de 0.1 sec
  SIM800L.println("AT+CNMI=1,2,0,0,0"); */ 
  //Configurar el SIM800L p/ que muestre msm por com. serie.
  
  #ifdef DEBUG_HX711
  Serial.begin(9600); //Inicializamos la primera comunicacion Serial.
  Serial.println("[HX7] Inicio del sensor HX711");
  #endif
  
  // Iniciar sensor
  bascula.begin(pinData, pinClk);
  calibracion();
}

void calibracion(){
  Serial.println("Añadir Masa de Calibración");
  //Serial.print();
  while(true){
    if (abs(bascula.read())< 8000)
    {
      bascula.set_scale(fact_calibracion); // Sin nada encima
      bascula.tare();
      bascula.set_offset();
      break;
    } 
    else 
    {
      relacion = 0;
      break;
    }
  }
  Serial.println("Calibration Complete");
}

void Weight(){
  
  // calculando masa
  #ifdef DEBUG_HX711
  Serial.print("[HX7] Leyendo: ");
  float peso = 0;

  if ( relacion == 0){
    peso = abs(bascula.get_units())/ 41000;
  }
  else{
    peso = abs(bascula.get_units());
  }

  if ( peso >= 40.0 ){
    digitalWrite(LEDV,HIGH);
    digitalWrite(LEDR,LOW);
    digitalWrite(LEDA,LOW);
  }
  if ( peso > 12.0 && peso < 30){
    digitalWrite(LEDA,HIGH);
    digitalWrite(LEDR,LOW);
    digitalWrite(LEDV,LOW);
  }
  if ( peso <= 12.0 ){
    digitalWrite(LEDR,HIGH);
    digitalWrite(LEDV,LOW);
    digitalWrite(LEDA,LOW);
  }
  
  Serial.print(peso,2);
  Serial.print(" Kg");
  Serial.print(" ");

  porcentaje = (peso/ref)*100;

  if ( porcentaje > 100 ){
    porcentaje = 100;
  }
  
  Serial.print( porcentaje,2);
  Serial.print(" %");
  Serial.println();
  delay(1000);

  // Reiniciar báscula
  estado = digitalRead(Button);
  
  if(estado == HIGH){
    calibracion();
  }

  #endif
  //delay(1.8e+7);//Cada 5 horas
  delay(1000);
}

void loop ()
{
  Weight();
}
