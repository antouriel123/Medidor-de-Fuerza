#include <stdint.h>

#ifndef _LoadCell_H_
#define _LoadCell_H_
/*     DECLARACION DE MACROS Y CONSTANTES     */
#define ADC_ADCDATA_R 0x00        //Este es el registro donde se guarda el resultado de la conversion del ADC
#define ADC_CONFIG0_R 0x01        //Registro de configuracion 0 
#define ADC_CONFIG1_R 0x02
#define ADC_CONFIG2_R 0x03
#define ADC_CONFIG3_R 0x04
#define ADC_IRQ_R 0x05
#define ADC_MUX_R 0x06
#define ADC_SCAN_R 0x07
#define ADC_TIMER_R 0x08
#define ADC_OFFSETCAL_R 0x09
#define ADC_GAINCAL_R 0x0A
#define ADC_RESERVED_B_R 0x0B
#define ADC_RESERVED_C_R 0x0C
#define ADC_LOCK_R 0x0D
#define ADC_RESERVED_E_R 0x0E     //MCP3564 Reserved E Register Address
#define ADC_CRCCFG_R 0x0F        
//Comandos rapidos 
#define CMD_STATIC_READ_CTRL 0x41 //Comando de Lectura
#define CMD_INC_WRITE_CTRL 0x42   //Comando de escritura
#define CMD_START_CONV_CTRL 0x68  //Comando para Inicia o Reiniciar conversion en el ADC

class LoadCell
{
  private: //Atributos y Metodos
    uint16_t CapMax;
    float Sens;
    double Offset;
    uint8_t VoltMax;
    uint8_t CS,IRQ;

    void MCP3561_Write(uint8_t Registro,uint32_t data);
    uint32_t MCP3561_Read(uint8_t Registro);
    void SPI_CNVRT(void);
  public:  //Metodos
    LoadCell(uint8_t _cs_,uint8_t _irq_);
    ~LoadCell();
    void Begin(uint16_t _capacidad_,float _sens_,uint8_t Vin);
    void getReading(uint32_t *data);
    void getAverageReading(double *data,uint8_t n);
    void getWeight(float *Weightdata);
    void getWeightAverage(float *Weightdata,uint8_t n);
};

#endif /*_MPU6050ESP32_H_*/