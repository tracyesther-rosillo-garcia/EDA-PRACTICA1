/*****************************************
 * Nombre: ExportarFichero
 * Argumentos: ANIMAL **Fichas:   Puntero al array de animales
 *             WINDOW *Wfichero: Ventana para capturar el nombre del fichero
 * Descripción: Lee el nombre del fichero que contendrá los datos a exportar. Exporta los datos de la estructura
 *              de todos los animales en el fichero.
 * Reglas de uso: 
 * Código de Retorno:
 * Programador: 
 *****************************************/

#include "Ficheros.h"

void ExportarFichero(ANIMAL **Fichas,WINDOW *Wfichero)
{
    FILE*archivo;
    char nombreFichero[256];
    int i;
    char *numAnimal,*clase,*orden,*genero,*especie,*peso, *cerebro;
    //Comprobar si hay animales para exportar 
    if(*Fichas == NULL || Estadisticas.NumeroFichas == 0){
        VentanaError("Error, no hay animales para exportar.");
        return;
    } else{
        //Pedimos el nombre al usuario
        mvwprintw(Wfichero,2,1,"Introduce el nombre del fichero destino:");
        wrefresh(Wfichero);
        echo();
        wgetstr(Wfichero, nombreFichero);
        noecho();

        //Abrimos el fichero
        archivo=fopen(nombreFichero,"w");
        if(!archivo){
            mvwprintw(Wfichero,3,1,"Error al abrir el archivo el fichero %s",nombreFichero);
            wrefresh(Wfichero);
            getch();
            return;
        }
        
        // escribir la cabecera de animales
        fprintf(archivo, "Clase;Orden;Genero;Especie;Peso;Tama�o;cerebro\n");
        //HE CAMBIADO HASTA AQUI, MIRAD TODO PORFA
        //guardar las estadisticas del dicso 
        for(i=0;i<Estadisticas.NumeroFichas;i++){
            if ((*Fichas)[i].Obra != NULL){
                obra = (*Fichas)[i].Obra;
            }
            if ((*Fichas)[i].ApellAutor != NULL){
                apellAutor = (*Fichas)[i].ApellAutor;
            }
            if ((*Fichas)[i].NomAutor != NULL){
                nomAutor = (*Fichas)[i].NomAutor;
            }
            if ((*Fichas)[i].Tonalidad != NULL){
                tonalidad = (*Fichas)[i].Tonalidad;
            }
            if ((*Fichas)[i].Opus != NULL){
                opus = (*Fichas)[i].Opus;
            }
            if ((*Fichas)[i].Duracion != NULL){
                duracion = (*Fichas)[i].Duracion;
            }
            fprintf(archivo,"%s;%s;%s;%s;%s;%s\n",obra, apellAutor, nomAutor, tonalidad, opus, duracion);
        }

        fclose(archivo);
        // Actualizar estadísticas
        if (Estadisticas.Fichero != NULL) {
            free(Estadisticas.Fichero);
            Estadisticas.Fichero = NULL;
        }

        Estadisticas.Fichero = strdup(nombreFichero);
        mvwprintw(Wfichero, 2, 1, "Se ha exportado con éxito");
        wrefresh(Wfichero);
        getch();
    }
}