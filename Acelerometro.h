#include <stdint.h>

#ifndef _Acelerometro_H_
#define _Acelerometro_H_

/*     DECLARACION DE VARIABLES GLOBALES     */
static int16_t Ax,Ay,Az;
#define dataLength 100

/*     DECLARACION DE PROTIPOS DE FUNCION     */
extern void Acelerometro_begin(void); 
extern void Read_Acelerometro(float *acelx,float *acely,float *acelz,double *Acel);
extern String EmpaquetarDatos(float *Acelx,float *Acely,float *Acelz,double *Acel);

#endif /* _TransAndRecep_H_ */