#include <Arduino.h>
#include <ArduinoJson.h>
#include "LoadCell.h"
#include "InstrumentoDePeso.h"

/*     DECLARACION DE TIPOS DE DATOS     */
LoadCell Sensor(2,15);

void InitInstrumentoDePeso(void)
{
  //Le indicamos la gancia y la velocidad de muestreo  
  Sensor.Begin(Capacidad,Sensibilidad,VoltajeMax); 
  //Escribimos en el puerto serial 
  Serial.print("Sensor inicializado!!");  
}
void LecturasDePeso(float *peso)
{
  for(uint16_t i=0;i<dataLength;i++)
  {
    //Serial.println("Tomando muestra de peso");
    //Leemos datos del sensor 
    Sensor.getWeight(&P);
    //Serial.println(P);
    //Lo guardamo en la variable 
    *(peso+i)=P;
    //Serial.println(*(peso+i));
  }
}
