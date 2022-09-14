#ifndef _InstrumentoDePeso_H_
#define _InstrumentoDePeso_H_

/*     DECLARACION DE MACROS Y CONSTANTES     */
#define Capacidad 500
#define Sensibilidad 0.002
#define VoltajeMax 12
#define dataLength 100

/*     DEFINICION DE VARIABLES GLOBALES     */
static float P;

/*     DEFINICION DE PROTOTIPOS DE FUNCION     */
extern void InitInstrumentoDePeso(void);
extern void LecturasDePeso(float *peso);

#endif /*_InstrumentoDePeso_H_*/