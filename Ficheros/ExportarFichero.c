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
    char *clase,*orden,*genero,*especie,*peso, *cerebro;
    //Comprobar si hay animales para exportar 
    if(*Fichas == NULL || Estadisticas.NumeroFichas == 0){
        VentanaError("Error, no hay animales para exportar.");
        return;
    } else{
        //Pedimos el nombre al usuario
        mvwprintw(Wfichero,2,1,"Nombre del fichero:");
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
        fprintf(archivo, "Clase;Orden;Genero;Especie;Peso;Cerebro\n");
    
        //guardar las estadisticas
        for(i=0;i<Estadisticas.NumeroFichas;i++){

            if ((*Fichas)[i].Clase != NULL){
                clase = (*Fichas)[i].Clase;
            } else {
                clase = "";
            }

            if ((*Fichas)[i].Orden != NULL){
                orden = (*Fichas)[i].Orden;
            } else {
                orden = "";
            }

            if ((*Fichas)[i].Genero != NULL){
                genero = (*Fichas)[i].Genero;
            } else {
                genero = "";
            }
            if ((*Fichas)[i].Especie != NULL){
                especie = (*Fichas)[i].Especie;
            } else {
                especie = "";
            }
            if ((*Fichas)[i].Peso != NULL){
                peso = (*Fichas)[i].Peso;
            } else {
                peso = "";
            }
            if ((*Fichas)[i].Cerebro != NULL){
                cerebro = (*Fichas)[i].Cerebro;
            } else {
                cerebro = "";
            }

            fprintf(archivo,"%s;%s;%s;%s;%s;%s\n",clase,orden,genero,especie,peso,cerebro);
        }

        fclose(archivo);
        // Actualizar estadísticas
        if (Estadisticas.Fichero != NULL) {
            free(Estadisticas.Fichero);
            Estadisticas.Fichero = NULL;
        }

        Estadisticas.Fichero = strdup(nombreFichero);
        mvwprintw(Wfichero, 2, 1, "Fichero exportado");
        wrefresh(Wfichero);
        getch();
    }
}