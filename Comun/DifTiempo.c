/*****************************************
 * Nombre: DifTiempo
 * Argumentos: struct timeval inicio:   Tiempo de inicio
 *             struct timeval fin:      Tiempo de fin
 * Descripción: Calcula los microsegundos de diferencia entre ambos tiempos
 * Reglas de uso: 
 * Código de Retorno: Microsegundos de diferencia
 * Programador: 
 *****************************************/

// Include del módulo funcional
#include "Comun.h"

int DifTiempo(struct timeval inicio,struct timeval fin)
{
    
    int diferencia;         // Microsegundos de diferencia

    // Se restan los microsegundos
    diferencia=fin.tv_usec-inicio.tv_usec;

    // Si es menor que 0 hay que añadir un segundo y quitarselo a inicio
    if (diferencia < 0) {
        diferencia += 1000000;
        fin.tv_sec--;
    }

    // Se añaden los segundos de diferencia
    diferencia=diferencia+(fin.tv_sec-inicio.tv_sec)*1000000;

    // Se devuelve el número de microsegundos de diferencia
    return(diferencia);
}