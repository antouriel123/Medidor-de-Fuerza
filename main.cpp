/*************************************************************************************************************************
 *Fecha: 24 de agosto 2022
 *Autor: Antonio Uriel
 *Descripcion: Programa que haces este primer ESP32 como servidor y va leer los datos que envie el otro cliente que es un
 *esp32(Acelerometro)
 *************************************************************************************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include "Clave.h"  
#include "Server.hpp"
#include "ESP32_Utils.hpp"
#include "InstrumentoDePeso.h"

void setup() {
  //Configuramos el puerto serial a 115200 baud 
	Serial.begin(19200);
  //Montamos el sistema de archivos 
  SPIFFS.begin();
  //Configuramos el sensor de peso
  InitInstrumentoDePeso();
  //Nos conectamos a la red de wifi 
  ConnectWiFi_STA();
  //Inicializamos el Servidor asincorno del ESP32
  InitServer();
}

void loop() {
}