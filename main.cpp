/*********************************************************************************************************************
 *Fecha: 24 de agosto del 2022
 *Autor: Antonio Uriel
 *Descripcion: Programa que captura los datos de aceleracion y los envia via http al servidor esp32.
 *********************************************************************************************************************/
/*    DIRECTIVAS DEL PROGRAMA    */
#include "TransAndRecep.h"
#include "Acelerometro.h"
#include "ESP32_WiFi.h"

/*    DECLRACION DE VARIABLES GLOBALES    */ 
float acelx[dataLength],acely[dataLength],acelz[dataLength];
double acel[dataLength];
String DatosAcel;
bool estado=false;

void setup() {
  //Inicializamos la conexion con el puerto serial 
  Serial.begin(19200);
  //Inicializamos Acelerometro 
  Acelerometro_begin();
  //Establecemos el wifi para comunicarnos con el servidor 
  ConectWiFi();
}

void loop() {
  //Leemos los dato en el acelerometro  
  Read_Acelerometro(&acelx[0],&acely[0],&acelz[0],&acel[0]);
  //Imprimos los datos capturados por el acelerometro 
  /*for(uint16_t j=0;j<dataLength;j++);
  {
    Serial.print(acelx[j]);
    Serial.print("\t");
    Serial.print(acely[j]);
    Serial.print("\t");
    Serial.print(acelz[j]);
    Serial.print("\t");
    Serial.println(acel[j]);
  } */
  //Empaquetamos los datos capturados 
  DatosAcel=EmpaquetarDatos(&acelx[0],&acely[0],&acelz[0],&acel[0]);
  //Serial.println(DatosAcel);
  //Lo enviamos al servidor
  EnviarDatosAlServidor(DatosAcel);
  delay(500);
}