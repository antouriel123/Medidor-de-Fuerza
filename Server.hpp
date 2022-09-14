/*     DIRECTIVAS DEL PROGRAMA     */
#include <Arduino.h>
#include "InstrumentoDePeso.h"

/*     DECLARACION DE VARIABLES GLOBALES     */
float acelx[dataLength],acely[dataLength],acelz[dataLength];
float acel[dataLength],Peso[dataLength];                      
bool AcceptData=false;      //Variable para indicar si acepta los datos del aceleremotro o no

/*     DECLARACION DE TIPOS DE DATOS     */
AsyncWebServer server(80);

void DatosAcel(AsyncWebServerRequest *request)
{
  //Variables Locales
  String input;
  int Size;
  DynamicJsonDocument doc(8192);
  Serial.println("Recibiendo Datos del Acelerometro");
  //Primero vemos si aceptamos o no los datos 
  if(AcceptData==true)
  {
    //Ahora capturamos el dato del cliente acelerometro
    Size=request->args();
    //input=request->args();
    Serial.println(Size);
    //Despues de capturar lo que nos envio el cliente acelerometro 
    DeserializationError error = deserializeJson(doc, input);
    //Verificamos si no hay algun error o algo asi en la informacion
    if(error) 
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    //Proseguimos a guardar el dato en el arreglo para despues enviarlo de nuevo 
    JsonArray Aceleracion_eje_x = doc["Aceleracion_eje_x"];
    for(uint16_t i=0;i<dataLength;i++)
    {
      Serial.println("Datos de aceleracion eje x:");
      acelx[i]=Aceleracion_eje_x[i]; //eje X
      Serial.println(acelx[i]);
    }
    //Datos de aceleracion eje y 
    JsonArray Aceleracion_eje_y = doc["Aceleracion_eje_y"];
    for(uint16_t i=0;i<dataLength;i++)
    {
      Serial.println("Datos de aceleracion eje y:");
      acely[i]=Aceleracion_eje_y[i]; //eje y
      Serial.println(acelx[i]);
    } 
    //Datos de aceleracion eje z
    JsonArray Aceleracion_eje_z = doc["Aceleracion_eje_z"];
    for(uint16_t i=0;i<dataLength;i++)
    {
      Serial.println("Datos de aceleracion eje z:");
      acely[i]=Aceleracion_eje_z[i]; //eje z
      Serial.println(acelx[i]);
    } 
    //Datos de aceleracion 
    JsonArray Aceleracion = doc["Aceleracion"];
    for(uint16_t i=0;i<dataLength;i++)
    {
      Serial.println("Datos de aceleracion eje z:");
      acel[i]=Aceleracion_eje_z[i]; //aceleracion
      Serial.println(acelx[i]);
    }
    //Respondemos a la peticion post 
    request->send(200,"text/plain","Aceptado"); 
    //Cambiamos de estado la variable
    AcceptData=false;        
  }
  else       //En caso contario no aceptamos los dato del acelerometro
  {
    //Respondemos a la peticion post 
    request->send(200,"text/plain","No Aceptado"); 
    //Cambiamos de estado la variable
    AcceptData=false;     
  }
}

String EmpaquetarDatosJson(float *Ax,float *Ay,float *Az,float *A,float *masa)
{
  String output;
  DynamicJsonDocument doc(8192);
  //Guardamos el dato de aceleracion en eje x
  JsonArray Aceleracion_eje_x = doc.createNestedArray("Aceleracion_eje_x");
  for(uint16_t i=0;i<dataLength;i++)
  {
    Aceleracion_eje_x.add(*(Ax+i));
  }
  //Guardamos el dato de aceleracion en eje y
  JsonArray Aceleracion_eje_y = doc.createNestedArray("Aceleracion_eje_y");
  for(uint16_t i=0;i<dataLength;i++)
  {
    Aceleracion_eje_y.add(*(Ay+i));
  }  
  //Guardamos el dato de aceleracion en eje z
  JsonArray Aceleracion_eje_z = doc.createNestedArray("Aceleracion_eje_z");
  for(uint16_t j=0;j<dataLength;j++)
  {
    Aceleracion_eje_z.add(*(Az+j));
  }
  //Guardamos los datos de aceleracion
  JsonArray Aceleracion = doc.createNestedArray("Aceleracion");
  for(uint16_t k=0;k<dataLength;k++)
  {
    Aceleracion.add(*(A+k));
  }
  //Guardamos los datos de peso
  JsonArray Peso = doc.createNestedArray("Peso");
  for(uint16_t l=0;l<dataLength;l++)
  {
    //Peso.add(*(masa+l));
    Peso.add(0.0);
  }

  //convertimos los datos a Json 
  serializeJson(doc, output);
  
  return output;
}

void TomarMuestras(AsyncWebServerRequest *request)
{
  String DatosPesoAcel;
  //Escribimos en el puerto serial 
  Serial.println("¡¡Peticion Recibida!!");
  //Cambiamos de estado la variable AcceptData para recibir los datos del acelerometro primero y despues el peso
  AcceptData=true;
  //Esperamos a recibir datos del sensor de acelerometro 
  while(AcceptData)
  {
    Serial.println("¡¡Esperando datos del acelerometro!!");
    delay(100);
  }
  //Ahora capturamos los datos de peso 
  //LecturasDePeso(&Peso[0]);
  //Mostramos los datos de las lecturas que obtuvimos del Acelerometro
  DatosPesoAcel=EmpaquetarDatosJson(&acelx[0],&acely[0],&acelz[0],&acel[0],&Peso[0]);
  //Enviamos el paquete 
  request->send(200,"text/json",DatosPesoAcel);
}

void BorrarDatos(float *peso,float *acelx,float *acely,float *acelz,float *acel)
{
  //Borramos los dato del arreglo 
  for(uint16_t i=0;i<dataLength;i++)
  {
    *(acelx+i)=0;
    *(acely+i)=0;
    *(acelz+i)=0;
    *(acel+i)=0.0;
    *(peso+i)=0.0;
  }
}

void ReiniciarVaraibles(AsyncWebServerRequest *request)
{
  String DatosPesoAcel;
  //Capturamos los 100 muestras del acelerometro 
  BorrarDatos(&Peso[0],&acelx[0],&acely[0],&acelz[0],&acel[0]);
  //Mostramos los datos de las lecturas que obtuvimos del Acelerometro
  DatosPesoAcel=EmpaquetarDatosJson(&acelx[0],&acely[0],&acelz[0],&acel[0],&Peso[0]);
  //Enviamos el paquete 
  request->send(200,"text/json",DatosPesoAcel);  
}

void InitServer()
{
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
	server.on("/start", HTTP_GET,TomarMuestras);

  server.on("/reset", HTTP_GET,ReiniciarVaraibles);

  server.on("/Acel",HTTP_POST,DatosAcel);

  server.onNotFound([](AsyncWebServerRequest *request) {
		request->send(400, "text/plain", "Not found");
	});

	server.begin();
	Serial.println("HTTP server started");
}
