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
    char input; //variable para almacenar la respuesta del usuario
    int i;
    //Comprobación 
    if (*Fichas == NULL || Estadisticas.NumeroFichas == 0) {
        VentanaError("Error, no hay discos para descartar en este fichero.");
        return;
    }
    input=VentanaSN("¿Seguro que quieres decartar todos los discos (S/N)?"); //llamo a la función para mostrar el mensaje y luego guardar la respuesta  

    if(input == 'N'){ //Si la respuesta es no (N) termina 
        return;
    }
 
    for(i=0; i<Estadisticas.NumeroFichas; i++){
        //liberar espacio uno por uno 
        if((*Fichas)[i].Obra!=NULL){
            free((*Fichas)[i].Obra);
        }
        if((*Fichas)[i].ApellAutor!=NULL){
            free((*Fichas)[i].ApellAutor);
        }
        if((*Fichas)[i].Duracion!=NULL){
            free((*Fichas)[i].Duracion);
        }
        if((*Fichas)[i].NomAutor!=NULL){
            free((*Fichas)[i].NomAutor);
        }
        if((*Fichas)[i].Opus!=NULL){
            free((*Fichas)[i].Opus);
        }
        if((*Fichas)[i].Tonalidad!=NULL){
            free((*Fichas)[i].Tonalidad);
        }
    }

    free(*Fichas); //liberar el espacio del array 
    *Fichas =NULL; //inicializar la estructura de discos 
    Estadisticas.MaxFichas=0; //Reinia el contador del número máximo de fichas
    Estadisticas.NumeroFichas=0; //Reinicia el contador del número de fichas
    
    //Mostrar el mensaje de éxito (mostrar discos descartados)
    mvwprintw(Wfichero,2,1,"Discos descartados.");
    wrefresh(Wfichero);
    getch();
    return;

}