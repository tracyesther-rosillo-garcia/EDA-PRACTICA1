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
#define MAX_BUF 1024    

void ImportarFichero(ANIMAL **Fichas,WINDOW *Wfichero,bool sumar)
{
    // variables
    FILE *fichero;
    struct timeval tiempoInicio, tiempoFinal;
    int i;
    int AnimalesLeidos = 0;
    int totalAnimales = 0;
    int animalesDescartados = 0;
    int animalesTratados = 0;
    char buffer[MAX_BUF];
    char nombreFichero[MAX_BUF]; // Guardamos el nombre aquí para no perderlo al leer el fichero
    int ancho = getmaxx(Wfichero); // calculamos el ancho de la ventana

    werase(Wfichero); // Limpiamos la ventana antes de dibujar

    DibujarGestionFichero(Wfichero); // Pintamos
    
    if (sumar) {
        mvwprintw(Wfichero, 0, (ancho - 27) / 2, "  Sumar Fichero de animales");
    } else {
        mvwprintw(Wfichero, 0, (ancho - 28) / 2, "  Importar Fichero de animales");
    }
    wrefresh(Wfichero); 
    wgetch(Wfichero); //Esperamos a que el usuario escriba antes de seguir

    // comprobamos el modo
    if (sumar) {
        if (Estadisticas.NumeroFichas == 0) { // si pedimos sumar pero no hay fichas da error
            mvwprintw(Wfichero, 2, 1, "Error, no hay fichas para sumar");
            wrefresh(Wfichero);
            getch();
            return;
        }
    } else {
        if (Estadisticas.NumeroFichas != 0) { // si importamos pero ya hay fichas da error
            mvwprintw(Wfichero, 2, 1, "Error, hay fichas existentes");
            wrefresh(Wfichero);
            getch();
            return;
        }
    } 

    // pedir por pantalla el fichero a abrir
    mvwprintw(Wfichero, 2, 1, "Nombre del fichero:");
    echo(); // habilitamos la visualización de lo que se escribe
    wgetstr(Wfichero, nombreFichero); // leemos la cadena del fichero
    noecho(); // deshabilitamos la visualización

    fichero = fopen(nombreFichero, "r"); // abre el fichero en modo lectura 'r'
    if (fichero == NULL) {
        mvwprintw(Wfichero, 4, 1, "Error al abrir el fichero: %s", strerror(errno));
        wrefresh(Wfichero);
        getch();
        return;
    }

    gettimeofday(&tiempoInicio, NULL); // guardamos el tiempo de inicio
    
    // Contamos las líneas del fichero
    while (fgets(buffer, MAX_BUF, fichero) != NULL) {
        AnimalesLeidos++;
    }

    // comprobamos lo que se ha leído
    if (AnimalesLeidos < 2) { // así lee la cabecera o nada
        mvwprintw(Wfichero, 4, 1, "Error, el fichero no contiene animales para importar.");
        wrefresh(Wfichero);
        getch();
        fclose(fichero);
        return;
    }

    totalAnimales = AnimalesLeidos - 1; // restamos 1 por la cabecera
    int posicionInicial = 0; 
    if (sumar) {
        posicionInicial = Estadisticas.NumeroFichas;
    } else {
        posicionInicial = 0;
    }


    ANIMAL *nuevasFichas = realloc(*Fichas, (posicionInicial + totalAnimales) * sizeof(ANIMAL));

    if (nuevasFichas == NULL) { // si realloc falla, no hacemos nada
        mvwprintw(Wfichero, 4, 1, "Error de memoria al redimensionar."); 
        wrefresh(Wfichero);
        getch();
        fclose(fichero);
        return;
    }

    *Fichas = nuevasFichas; // actualizamos el puntero
    Estadisticas.MaxFichas = posicionInicial + totalAnimales; // actualizamos capacidad máxima

    rewind(fichero); // volvemos al inicio del fichero para leer las líneas
    bool esCabecera = true;

    while (fgets(buffer, MAX_BUF, fichero) != NULL) {
        if (esCabecera) { 
            esCabecera = false;
            continue; // salto la cabecera
        }

        i = strlen(buffer); // longitud de la línea actual
        while(i>0){
            if (i > 0 && (buffer[i - 1] == '\n' || buffer[i - 1] == '\r')) { 
                buffer[i - 1] = '\0';
                i--; // elimina salto de línea final
            }else{
                break;
            }
        }

        char *buffer_ptr = buffer; // puntero auxiliar para strsep

        // Separamos exactamente las 6 columnas del CSV original
        char *clase_buf   = strsep(&buffer_ptr, ";");
        char *orden_buf   = strsep(&buffer_ptr, ";");
        char *genero_buf  = strsep(&buffer_ptr, ";");
        char *especie_buf = strsep(&buffer_ptr, ";");
        char *peso_buf    = strsep(&buffer_ptr, ";");
        char *cerebro_buf = strsep(&buffer_ptr, ";");

        // Comprobamos que el género (campo obligatorio) no esté vacío
        if (genero_buf == NULL || *genero_buf == '\0') {
            animalesDescartados++;
            continue; // descartamos esta ficha y pasamos a la siguiente
        }

        if (*genero_buf == '\0') {
            animalesDescartados++;
            continue; // descartamos esta ficha y pasamos a la siguiente
        }


        // Asignamos los 6 campos a la estructura ANIMAL
        if (clase_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Clase = strdup(clase_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Clase = strdup("");
        }

        if (orden_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Orden = strdup(orden_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Orden = strdup("");
        }

        if (genero_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Genero = strdup(genero_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Genero = strdup("");
        }

        if (especie_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Especie = strdup(especie_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Especie = strdup("");
        }

        if (peso_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Peso = strdup(peso_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Peso = strdup("");
        }

        if (cerebro_buf != NULL) {
            (*Fichas)[posicionInicial + animalesTratados].Cerebro = strdup(cerebro_buf);
        } else {
            (*Fichas)[posicionInicial + animalesTratados].Cerebro = strdup("");
        }

        animalesTratados++; // incrementamos contador de animales tratados
    }

    fclose(fichero);
    gettimeofday(&tiempoFinal, NULL);

    // Actualizamos las estadísticas
    Estadisticas.TiempoCarga += DifTiempo(tiempoInicio, tiempoFinal);

    if (Estadisticas.Fichero != NULL) {
        free(Estadisticas.Fichero);
    }
    Estadisticas.Fichero = strdup(nombreFichero);
    Estadisticas.NumeroFichas = posicionInicial + animalesTratados;

    // Limpiamos y redibujamos la pantalla final
    werase(Wfichero);
    DibujarGestionFichero(Wfichero);
    
    if (sumar) {
        mvwprintw(Wfichero, 0, (ancho - 27) / 2, "  Sumar Fichero de animales");
    } else {
        mvwprintw(Wfichero, 0, (ancho - 28) / 2, "  Importar Fichero de animales");
    }

    mvwprintw(Wfichero, 2, 1, "Nombre del fichero: %s", Estadisticas.Fichero);
    mvwprintw(Wfichero, 3, 1, "Fichas tratadas: %-6d Fichas descartadas: %-6d", animalesTratados, animalesDescartados);
    wrefresh(Wfichero);

    VentanaError("Fichero importado correctamente");
    getch();
    return;
}