#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "Clave.h"
#include "ESP32_WiFi.h"

/*     DECLARACION DE TIPO DE DATO     */
WiFiMulti wifiMulti;

void ConectWiFi(void) {
  //Configuramos el puerto serial
  Serial.begin(19200);
  //Establecemos el modo del WiFi
  WiFi.mode(WIFI_STA);
  //En dado caso que estuvo o esta conectado los desconectamos por completo de la red
  WiFi.disconnect();
  //Esperamos a que se desconecte
  delay(1000);
  //Escribimos en el puerto serial para indicar que vamos a conectar en alguna red disponible dentro de Clave.h
  wifiMulti.addAP(SSID_1, PASSWORD_1);
  wifiMulti.addAP(SSID_2, PASSWORD_2);
  Serial.println("Conectando a Wifi...");
 while(wifiMulti.run() != WL_CONNECTED) 
  {
    delay(100);  
    Serial.print('.');   
  }
  //Enviamos un mensaje de que la conexion fue un exito
  Serial.println("");
  Serial.println("¡¡WiFi conectado!!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}