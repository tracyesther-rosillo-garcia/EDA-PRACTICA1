/*****************************************
 * Nombre: ImortarFichero
 * Argumentos: ANIMAL **Fichas:   Puntero al array de animales
 *             WINDOW *Wfichero: Ventana para capturar el nombre del fichero
 *             bool sumar:       Si es true las fichas de animales se añaden a las existentes,
 *                               si es false la lista de animales tiene que estar vacía.
 * Descripción: Lee el nombre del fichero que contendrá los datos a importar. Carga los animales del fichero
 *              en el array de animales, dimensionándolo si es necesario y descartando los que no tengan título o autor.
 * Reglas de uso: 
 * Código de Retorno:
 * Programador:
 *****************************************/

#include "Ficheros.h"

void ImportarFichero(ANIMAL **Fichas,WINDOW *Wfichero,bool sumar)
{
        //variables
    FILE *fichero;
    char **Lineas = NULL;
    struct timeval tiempoInicio, tiempoFinal;
    char *token;
    int i, j;                                       //contadores
    int x, y;                                       //coordenadas para la ventana  
    int contador = 0;
    int numeroLineas = 0;
    int animalesDescartados = 0 ;
    int animalesTratados = 0;
    int totalAnimales = 0;
    int AnimalesLeidos = 0;
    char buffer[MAX_BUF];
    int ancho = getmaxx(Wfichero);      //calculamos el ancho de la ventana

    werase(Wfichero);        //Limpiamos la ventana antes de dibujar

    DibujarGestionFichero(Wfichero);        //Pintamos
    mvwprintw(Wfichero, 0, (ancho - 27) / 2 , "  Importar Fichero de Discos");       //agregalos el titulo faltante    
    wrefresh(Wfichero);     
    wgetch(Wfichero);        //mantiene la ventana hasta que el usuario haga algo


    //comprobamos el modo
    if(sumar) {
        if(Estadisticas.NumeroFichas==0) {      //si pedimos sumar pero no hay fichas da error
            mvwprintw(Wfichero, 2, 1, "Error, no hay fichas para sumar");
            wrefresh(Wfichero);
            getch();
            return;
        }
    } else {
        if(Estadisticas.NumeroFichas!=0) {      //si sumamos pero ya hay fichas da error
            mvwprintw(Wfichero, 2, 1, "Error, hay fichas existentes");
            wrefresh(Wfichero);
            getch();
            return;
        }
    } 

    //pedir por pantalla el fichero a abrir
    mvwprintw(Wfichero, 2, 1, "Introduzca el nombre del fichero a abrir:");
    echo();             //habilitamos la visualización de lo que se escribe
    wgetstr(Wfichero, buffer);      //leemos la cadena del fichero
    noecho();           //deshabilitamos la visualización


    fichero = fopen(buffer, "r");       //abre el fichero en modo lectura 'r'
    if (fichero == NULL) {
        mvwprintw(Wfichero, 4, 1, "Error al abrir el fichero: %s", strerror(errno));
        wrefresh(Wfichero);
        getch();
        return;
    }

    gettimeofday(&tiempoInicio, NULL);      //guardamos el tiempo de inicio
    
    while(fgets(buffer, MAX_BUF,fichero) != NULL) {
        i = strlen(buffer);       //calculamos la longitud de la linea
        AnimalesLeidos++;           //incrementamos el contador de animales leidos
    }

    //comprobamos lo que se ha leido
    if(AnimalesLeidos < 2) { //asi lee la cabecera o nada
        mvwprintw(Wfichero, 4, 1, "Error, el fichero no contiene animales para importar.");
        wrefresh(Wfichero);
        getch();
        fclose(fichero);
        return;
    }


    totalAnimales = AnimalesLeidos - 1 ; //restamos 1 por la cabecera
    ANIMAL *nuevasFichas = realloc(*Fichas, (Estadisticas.NumeroFichas + totalAnimales) * sizeof(ANIMAL));  //redimensionamos el array de animales

    Estadisticas.MaxFichas = Estadisticas.NumeroFichas + totalAnimales; //actualizamos la capacidad maxima 

    if(nuevasFichas == NULL) {      //si realloc falla, no hacemos nada
        mvwprintw(Wfichero, 4, 1, "Error de memoria al redimensionar los discos."); 
        wrefresh(Wfichero);
        getch();
        fclose(fichero);
        return;
    }
    *Fichas = nuevasFichas;     //si realloc tiene éxito, actualizamos el puntero



    rewind(fichero); //volvemos al inicio del fichero para leer las lineas
    AnimalesLeidos = 0; //reiniciamos el contador de animales leidos para la importación

    while(fgets(buffer, MAX_BUF, fichero) != NULL) {
        if(AnimalesLeidos == 0) { //si es la primera linea 
            AnimalesLeidos++;
            continue;           //salto la cabecera
        }
        i = strlen(buffer);     //longitud de la linea actual
        if(buffer[i-1] == '\n') {   //elimina salto de linea final
            buffer[i-1] = '\0'; 
        }

        //separamos los tokens por numero del animal, clase, orden, genero, especie, peso y cerebro
        
        char *buffer_ptr = buffer;      //puntero auxiliar para strsep

        char *numAnimal_buf = strsep(&buffer_ptr, ";");
        char *clase_buf = strsep(&buffer_ptr, ";");
        char *orden_buf = strsep(&buffer_ptr, ";");
        char *genero_buf = strsep(&buffer_ptr, ";");
        char *especie_buf = strsep(&buffer_ptr, ";");
        char *peso_buf = strsep(&buffer_ptr, ";");
        char *cerebro_buf = strsep(&buffer_ptr, ";");

        //comprobamos que los campos obligatorios no estén vacíos
        if(*genero_buf == 0){ //si no existe este valor, no se agrega
            animalesDescartados++;
            continue; //descartamos esta ficha y pasamos a la siguiente
        }

        //asignamos los valores a la nueva ficha
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Numero = strdup(numAnimal_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Clase = strdup(clase_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Orden = strdup(orden_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Genero = strdup(genero_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Especie = strdup(especie_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Peso = strdup(peso_buf);
        (*Fichas)[animalesTratados + AnimalesLeidos - 1].Cerebro = strdup(cerebro_buf);

        animalesTratados++;                       //incrementamos contador de animales tratados
    }

    fclose(fichero);
    gettimeofday(&tiempoFinal, NULL);

    //actualizamos las estadisticas
    Estadisticas.TiempoCarga += DifTiempo(tiempoInicio, tiempoFinal);
    Estadisticas.Fichero = strdup(buffer);

    Estadisticas.NumeroFichas = animalesTratados + AnimalesLeidos - 1 ; //total fichas = importadas + leidas - cabecera
    wrefresh(Wfichero);

    //mostramos resultados
    mvwprintw(Wfichero, 2, 1, "Nombre del fichero: %s         .", Estadisticas.Fichero);
    mvwprintw(Wfichero, 3, 1, "Número de fichas: %d", Estadisticas.NumeroFichas);
    mvwprintw(Wfichero, 4, 1, "Fichas descartadas: %d", animalesDescartados);
    wrefresh(Wfichero);
    VentanaError("Fichero importado correctamente");
    getch();
    return;
}