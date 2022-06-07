#include <SoftwareSerial.h> //Incluimos la libreria Software Serial

#include "HX711.h"  // Libreria de galgas
#define DEBUG_HX711
#define fact_calibracion 31100.0  // Factor de calibración pesa inicial

void calibracion();
void Weight();
