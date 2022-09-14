#include <ArduinoJson.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Acelerometro.h"

/*    DECLARACION DE TIPOS DE DATOS    */
MPU6050 accelerometer;

void Acelerometro_begin(void)
{
  Wire.begin();                  //Iniciando I2C  
  accelerometer.initialize();    //Iniciando el sensor

  if (accelerometer.testConnection())
  { 
    Serial.println("Sensor iniciado correctamente");
  }
  else 
  {
    Serial.println("Error al iniciar el sensor");
  }
  //Establecemos la aceleracion maxima
  accelerometer.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
} 

void Read_Acelerometro(float *acelx,float *acely,float *acelz,double *Acel)
{
  for(uint16_t i=0;i<dataLength;i++)
  {
    accelerometer.getAcceleration(&Ax,&Ay,&Az);
    //Multiplicamos por la sensibilidad seleccionada que es 2048LSB/g
    *(acelx+i)=Ax*(9.81/2048.0);    
    *(acely+i)=Ay*(9.81/2048.0);
    *(acelz+i)=Az*(9.81/2048.0);
    //Calculamos el modulo de la aceleracion cada lectura
    *(Acel+i)=sqrt(pow(*(acelx+i),2)+pow(*(acely+i),2)+pow(*(acelz+i),2));
  }
}

String EmpaquetarDatos(float *Acelx,float *Acely,float *Acelz,double *Acel)
{
  String output;
  DynamicJsonDocument doc(8192);
  //Guardamos los datos de aceleracion en el eje x
  JsonArray Aceleracion_eje_x = doc.createNestedArray("Aceleracion_eje_x");
  for(uint16_t i=0;i<dataLength;i++)
  {
    Aceleracion_eje_x.add(*(Acelx+i));
  }
  //Guardamos los datos de aceleracion en el eje y
  JsonArray Aceleracion_eje_y = doc.createNestedArray("Aceleracion_eje_y");
  for(uint16_t j=0;j<dataLength;j++)
  {
    Aceleracion_eje_y.add(*(Acely+j));
  }
  //Guardamos los datos de aceleracion en el eje z
  JsonArray Aceleracion_eje_z = doc.createNestedArray("Aceleracion_eje_z");
  for(uint16_t k=0;k<dataLength;k++)
  {
    Aceleracion_eje_z.add(*(Acelz+k));
  }
  //Guardamos los datos de aceleracion 
  JsonArray Aceleracion = doc.createNestedArray("Aceleracion");
  for(uint16_t l=0;l<dataLength;l++)
  {
    Aceleracion.add(*(Acel+l));
  }
  //Convertimos el documento a Json 
  serializeJson(doc, output);

  return output;
}
