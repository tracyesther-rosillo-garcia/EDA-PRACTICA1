/*****************************************
 * Nombre: DescartarFichero
 * Argumentos: ANIMAL **Fichas:   Puntero al array de animales
 *             WINDOW *Wfichero: Ventana para mostrar el nombre del fichero
 * Descripción: Muestra los datos del fichero y los animales almacenados y descarta todos
 *              los animales en memoria y libera la memoria asignada.
 * Reglas de uso: 
 * Código de Retorno:
 * Programador: 
 *****************************************/

#include "Ficheros.h"

void DescartarFichero(ANIMAL **Fichas,WINDOW *Wfichero)
{
    char input; // variable para almacenar la respuesta del usuario
    int i;

    // Comprobación 
    if (*Fichas == NULL || Estadisticas.NumeroFichas == 0) {
        VentanaError("Error, no hay animales para descartar en este fichero.");
        return;
    }

    input = VentanaSN("¿Seguro que quieres descartar todos los animales (S/N)?"); // llamo a la función para mostrar el mensaje y luego guardar la respuesta  

    if (input == 'N' || input == 'n') { // Si la respuesta es no termina 
        return;
    }
 
    for (i = 0; i < Estadisticas.NumeroFichas; i++) {
        // liberar espacio uno por uno 
        if ((*Fichas)[i].Clase != NULL) {
            free((*Fichas)[i].Clase);
        }
        if ((*Fichas)[i].Orden != NULL) {
            free((*Fichas)[i].Orden);
        }
        if ((*Fichas)[i].Genero != NULL) {
            free((*Fichas)[i].Genero);
        }
        if ((*Fichas)[i].Especie != NULL) {
            free((*Fichas)[i].Especie);
        }
        if ((*Fichas)[i].Peso != NULL) {
            free((*Fichas)[i].Peso);
        }
        if ((*Fichas)[i].Cerebro != NULL) {
            free((*Fichas)[i].Cerebro);
        }
    }

    free(*Fichas); // liberar el espacio del array 
    *Fichas = NULL; // inicializar la estructura de animales 

    // liberamos el nombre del fichero para evitar fuga de memoria
    if (Estadisticas.Fichero != NULL) {
        free(Estadisticas.Fichero);
        Estadisticas.Fichero = NULL;
    }

    Estadisticas.MaxFichas = 0; // Reinicia el contador del número máximo de fichas
    Estadisticas.NumeroFichas = 0; // Reinicia el contador del número de fichas
    
    VentanaError("Animales descartados.");
}