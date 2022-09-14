#include <Arduino.h>

#ifndef _TransAndRecep_H_
#define _TransAndRecep_H_

/*    DECLARACION DE CONSTANTES Y MACROS    */
static const char *hostESP32Server = "http://192.168.100.126/";     //Este es el host o la direccion al servidor para enviar datos 
                                                                     //hacia el servidor
                                                                     /*     DECLARACION DE CONSTANTES Y MACROS     */
/*     PROTOTIPOS DE FUNCION     */
extern void EnviarDatosAlServidor(String paquete);

#endif /* _TransAndRecep_H_ */                                                            