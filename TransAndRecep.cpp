#include "WiFi.h"
#include "Clave.h"
#include "TransAndRecep.h"
#include "WiFiClient.h"    //Con esta libreria podremos inicializar la conexion
#include "HTTPClient.h"    //y con esta libreria es la que podremos manejar todas las peticiones 
#include <ArduinoJson.h>

/*    DECLARACION DE CONSTANTES Y MACROS    */
const char *host = "http://192.168.100.195/";
#define dataLength 100

/*    DECLARACION DE VARIABLES GLOBALES    */          
String Link;             //estas variables se ocupan para crear el Link y el otro para e dato porque la estructura se divide en

/*    DECLARACION DE TIPOS DE DATOS    */
WiFiClient cliente;
HTTPClient http;

void EnviarDatosAlServidor(String paquete)
{
  //Constantes y macros locales
  String ArchivoOrigen="Acel";
  //Variables globales
  String Link;
  //Construimos el link
  Link=host+ArchivoOrigen;
  //Escribimos en el puerto serial 
  Serial.print("Solicitud de peticion: ");
  Serial.println(Link);
  Serial.print("post data:");
  Serial.println(paquete);
  //Iniciar el objeto Http con el cliente y el link que nos vamos a conectar que es el host
  http.begin(cliente,Link);
  //Agregamos la cabezera al contenido empleando la funcion addheader(); y en los argumento indicando el tipo de dato que es TEXT/plain
  //o texto plano.
  http.addHeader("Content-Type","text/Json");
  //Ahora enviamos el dato empleando la siguiente funcion con los datos a enviar, devolviendo un numero como el anterior funcion
  int32_t httpCode=http.POST(paquete);
  //Ahora vamos a recibir la cadena que es la misma funcion empleada en get  
  String payload=http.getString();
  //Y lo imprimimos en el puerto serial 
  Serial.print("Codigo de respuesta: ");
  Serial.println(httpCode);
  Serial.print("Respuesta del servidor: ");
  Serial.println(payload);
  //Finalizamos la comunicacion con el Servidor
  http.end();
}
