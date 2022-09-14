/*     DIRECTIVAS PARA INCLUIR ARCHIVOS     */
#include <Arduino.h>
#include <SPI.h>
#include "LoadCell.h"

LoadCell::LoadCell(uint8_t _cs_,uint8_t _irq_)
{
  //Guardamos los pines de conexion del esclavo
  CS=_cs_;
  IRQ=_irq_;
  //Configuramos el pin de CS 
  pinMode(_cs_,OUTPUT);
  //y lo dejamos deshabilitados
  digitalWrite(_cs_,HIGH);
  //Configuramos el pin IRQ como entrada digital y su respectiva resistencia de pull up
  pinMode(_irq_,INPUT_PULLUP);
  //Inicializamos el bus SPI
  SPI.begin(); 
}

LoadCell::~LoadCell()
{
}

void LoadCell::Begin(uint16_t _capacidad_,float _sens_,uint8_t Vin)
{
  //Guardamos los parametro del sensor de fuerza empleado 
  CapMax=_capacidad_;
  Sens=_sens_;
  VoltMax=Vin;
  //Configuramos el ADC 
  //Le indicamos al ADC que ocupe el voltaje de referencia externa, empleando el oscilador interno y que el ADC inicie en modo Standby
  MCP3561_Write(ADC_CONFIG0_R,0x00000062);
  //Indicamos al ADC que el AMCLK=MCLK con una velocidad de Muestreo de 38400 muestras/s 
  MCP3561_Write(ADC_CONFIG1_R,0x00000000);
  //Configuramos que el ADC tenga la ganancia maxima con AZ_MUX=1.
  MCP3561_Write(ADC_CONFIG2_R,0x000000A7);
  //Configuramos el ADC en modo de conversion one-Shot con el formato de 24 bits  
  MCP3561_Write(ADC_CONFIG3_R,0x00000080); 
  //Habilitamos los comando rapidos y Activamos la interrupcion por el pin IRQ
  MCP3561_Write(ADC_IRQ_R,0x000000003);
  //MUX --> VIN+ = CH0, VIN- = AGND --> (0b00000008).
  MCP3561_Write(ADC_MUX_R,0x00000008);
  //SCAN --> Disabled.
  MCP3561_Write(ADC_SCAN_R,0x00000000);
  //TIMER --> Disabled.
  MCP3561_Write(ADC_TIMER_R,0x00000000);
  //OFFSETCAL --> Disabled
  MCP3561_Write(ADC_OFFSETCAL_R,0x00000000);
  //GAINCAL --> Disabled.
  MCP3561_Write(ADC_GAINCAL_R,0x00000000);
}

void LoadCell::getReading(uint32_t *data)
{
  //Inicia la conversion para obtener el dato medido
  SPI_CNVRT();
  //Leemos el dato en el Registro ADCDATA
  *data=(uint32_t)(0x7FFFFFFF&MCP3561_Read(ADC_ADCDATA_R)); 
}

void LoadCell::getAverageReading(double *data,uint8_t n)
{
  double sum=0;
  for(uint8_t i=0;i<n;i++)
  {
    //Inicia la conversion para obtener el dato medido
    SPI_CNVRT();
    //Leemos el dato en el Registro ADCDATA
    sum+=MCP3561_Read(ADC_ADCDATA_R);
  }
  //Calculamos el promedio de los datos leidos
  *data=sum/n;
}

void LoadCell::getWeight(float *Weightdata)
{
  //Variables locales
  double data;

  //Inicia la conversion para obtener el dato medido
  SPI_CNVRT();
  //Leemos el dato en el Registro ADCDATA
  data=(long)(0x7FFFFFFF&MCP3561_Read(ADC_ADCDATA_R)); 
  //Encontramos ahora sacamos el valor del voltaje apartir del valor leido en el ADC
  *Weightdata=0.000059209*data;
}

void LoadCell::getWeightAverage(float *Weightdata,uint8_t n)
{
  //Variables Locales
  double data,sum=0;
  
  for(uint8_t i=0;i<n;i++)
  {
    //Inicia la conversion para obtener el dato medido
    SPI_CNVRT();
    //Leemos el dato en el Registro ADCDATA
    sum+=MCP3561_Read(ADC_ADCDATA_R);
  }
  //Calculamos el promedio de los datos leidos
  data=sum/n;
  //Encontramos ahora sacamos el valor del voltaje apartir del valor leido en el ADC
  *Weightdata=0.000059209*data;
}

void LoadCell::MCP3561_Write(uint8_t Registro,uint32_t data)
{
 //Declaramos varables internos
  uint8_t COMANDO,DATA_LOW,DATA_HIGH,DATA_UPPER;

  //Separamos las variables
  DATA_LOW=(uint8_t)(data&0x000000FF);
  DATA_HIGH=(uint8_t)((data&0x0000FF00)>>8);
  DATA_UPPER=(uint8_t)((data&0x00FF0000)>>16);
  //Extraigo la dirección de registro MCP3564 para Write-CMD
  COMANDO=(uint8_t)(Registro<<2)|CMD_INC_WRITE_CTRL;

  if(Registro==ADC_CONFIG0_R||Registro==ADC_CONFIG1_R||Registro==ADC_CONFIG2_R||Registro==ADC_CONFIG3_R||Registro==ADC_IRQ_R||Registro==ADC_MUX_R||Registro==ADC_LOCK_R)
  {
    //Enviamos el dato por el bus SPI. Configuramo el bus SPI
    SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
    //Inicia la comunicacion con el esclavo
    digitalWrite(CS,LOW); 
    //Esperamos 1 us para que se estabilice el bus
    delayMicroseconds(1);
    //Envia los datos hacial el ADC
    SPI.transfer(COMANDO);
    SPI.transfer(DATA_LOW);
    //Esperamos 1 us para que termine el bus 
    delayMicroseconds(1);
    //Termina la comunicacion con el esclavo
    digitalWrite(CS,HIGH);
    SPI.endTransaction();
  }
  else if (Registro==ADC_SCAN_R||Registro==ADC_TIMER_R||Registro==ADC_OFFSETCAL_R||Registro==ADC_GAINCAL_R) 
  {
    //Enviamos el dato por el bus SPI. Configuramo el bus SPI
    SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
    //Inicia la comunicacion con el esclavo
    digitalWrite(CS,LOW); 
    //Esperamos 1 us para que se estabilice el bus
    delayMicroseconds(1);
    //Envia los datos hacial el ADC
    SPI.transfer(COMANDO);
    SPI.transfer(DATA_UPPER);
    SPI.transfer(DATA_HIGH);
    SPI.transfer(DATA_LOW);
    //Esperamos 1 us para que termine el bus 
    delayMicroseconds(1);
    //Termina la comunicacion con el esclavo
    digitalWrite(CS,HIGH);
    SPI.endTransaction();
  }
}

uint32_t LoadCell::MCP3561_Read(uint8_t Registro)
{
  //VARIABLES GLOBALES
  uint8_t COMANDO,DATA_LOW,DATA_HIGH,DATA_UPPER;
  uint32_t ADCdata;
  //Extraigo la dirección de registro MCP3564 para Write-CMD
  COMANDO=(uint8_t)(Registro<<2)|CMD_STATIC_READ_CTRL;
  if(Registro==ADC_CONFIG0_R||Registro==ADC_CONFIG1_R||Registro==ADC_CONFIG2_R||Registro==ADC_CONFIG3_R||Registro==ADC_IRQ_R||Registro==ADC_MUX_R||Registro==ADC_RESERVED_C_R ||Registro==ADC_LOCK_R)
  {
    //Enviamos el dato por el bus SPI. Configuramo el bus SPI
    SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
    //Inicia la comunicacion con el esclavo
    digitalWrite(CS,LOW); 
    //Esperamos 1 us para que se estabilice el bus
    delayMicroseconds(1);
    //Envia los datos hacial el ADC
    SPI.transfer(COMANDO);
    DATA_LOW=SPI.transfer(0);
    //Esperamos 1 us para que termine el bus 
    delayMicroseconds(1);
    //Termina la comunicacion con el esclavo
    digitalWrite(CS,HIGH);
    SPI.endTransaction();
  }
  else if(Registro==ADC_ADCDATA_R||Registro==ADC_SCAN_R||Registro==ADC_TIMER_R||Registro==ADC_OFFSETCAL_R||Registro==ADC_GAINCAL_R||Registro==ADC_RESERVED_B_R) 
  {
    //Enviamos el dato por el bus SPI. Configuramo el bus SPI
    SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
    //Inicia la comunicacion con el esclavo
    digitalWrite(CS,LOW); 
    //Esperamos 1 us para que se estabilice el bus
    delayMicroseconds(1);
    //Envia los datos hacial el ADC
    SPI.transfer(COMANDO);
    DATA_UPPER=SPI.transfer(0);
    DATA_HIGH=SPI.transfer(0);
    DATA_LOW=SPI.transfer(0);
    //Esperamos 1 us para que termine el bus 
    delayMicroseconds(1);
    //Termina la comunicacion con el esclavo
    digitalWrite(CS,HIGH);
    SPI.endTransaction();
  } 
  //Guardamos el dato en la varible para retornarlo
  ADCdata=((DATA_UPPER<<16)|(DATA_HIGH<<8)|DATA_LOW);

  return ADCdata;
}

void LoadCell::SPI_CNVRT(void)
{
  //Variable global
  uint8_t ADCstatus;
  //Enviamos el comando rapido para iniciar la conversion 
  SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
  //Inicia la comunicacion con el esclavo
  digitalWrite(CS,LOW); 
  //Esperamos 1 us para que se estabilice el bus
  delayMicroseconds(1);
  //forzamos el inicio de conversion
  ADCstatus=SPI.transfer(CMD_START_CONV_CTRL);
  //Esperamos 1 us para que termine el bus 
  delayMicroseconds(1);
  //Termina la comunicacion con el esclavo
  digitalWrite(CS,HIGH);
  SPI.endTransaction();
  //Esperamo a que termine la conversion con el ADC
  while ((digitalRead(IRQ)));    
  
}