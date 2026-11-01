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
        mvwprintw(Wfichero, 0, (ancho - 31) / 2, "   Sumar Fichero de animales   ");
    } else {
        mvwprintw(Wfichero, 0, (ancho - 32) / 2, "  Importar Fichero de animales  ");
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
    int posicionInicial = sumar ? Estadisticas.NumeroFichas : 0;
    int numeroAnterior = Estadisticas.NumeroFichas;

    /*
    * Reservamos espacio para los animales actuales más los nuevos.
    * Se resta la cabecera del CSV porque no es un animal.
    */
    int capacidadNecesaria = posicionInicial + (AnimalesLeidos - 1);

    ANIMAL *nuevasFichas = realloc(*Fichas,capacidadNecesaria * sizeof(ANIMAL));

    if (nuevasFichas == NULL) {
        mvwprintw(Wfichero, 4, 1,"Error de memoria al redimensionar.");
        wrefresh(Wfichero);
        getch();
        fclose(fichero);
        return;
    }

    *Fichas = nuevasFichas;

    rewind(fichero);

    bool esCabecera = true;
    int animalesTratados = 0;

    while (fgets(buffer, MAX_BUF, fichero) != NULL) {
        if (esCabecera) {
            esCabecera = false;
            continue;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';

        char *buffer_ptr = buffer;

        char *clase_buf   = strsep(&buffer_ptr, ";");
        char *orden_buf   = strsep(&buffer_ptr, ";");
        char *genero_buf  = strsep(&buffer_ptr, ";");
        char *especie_buf = strsep(&buffer_ptr, ";");
        char *peso_buf    = strsep(&buffer_ptr, ";");
        char *cerebro_buf = strsep(&buffer_ptr, ";");

        if (genero_buf == NULL || *genero_buf == '\0') {
            animalesDescartados++;
            continue;
        }

        ANIMAL *animal = &(*Fichas)[posicionInicial + animalesTratados];

        animal->Clase = clase_buf != NULL ? strdup(clase_buf) : strdup("");
        animal->Orden = orden_buf != NULL ? strdup(orden_buf) : strdup("");
        animal->Genero = strdup(genero_buf);
        animal->Especie = especie_buf != NULL ? strdup(especie_buf) : strdup("");
        animal->Peso = peso_buf != NULL ? strdup(peso_buf) : strdup("");
        animal->Cerebro = cerebro_buf != NULL ? strdup(cerebro_buf) : strdup("");

        animalesTratados++;
    }

    Estadisticas.NumeroFichas = numeroAnterior + animalesTratados;
    Estadisticas.MaxFichas = capacidadNecesaria;

    fclose(fichero);
    gettimeofday(&tiempoFinal, NULL);

    // Actualizamos las estadísticas
    Estadisticas.TiempoCarga += DifTiempo(tiempoInicio, tiempoFinal);

    if (Estadisticas.Fichero != NULL) {
        free(Estadisticas.Fichero);
    }
    Estadisticas.Fichero = strdup(nombreFichero);

    // Limpiamos y redibujamos la pantalla final
    werase(Wfichero);
    DibujarGestionFichero(Wfichero);
    
    if (sumar) {
        mvwprintw(Wfichero, 0, (ancho - 31) / 2, "   Sumar Fichero de animales   ");
    } else {
        mvwprintw(Wfichero, 0, (ancho - 32) / 2, "  Importar Fichero de animales  ");
    }

    mvwprintw(Wfichero, 2, 1, "Nombre del fichero: %s               ", Estadisticas.Fichero);
    mvwprintw(Wfichero, 3, 1, "Fichas tratadas: %-6d Fichas descartadas: %-6d               ", animalesTratados, animalesDescartados);
    wrefresh(Wfichero);

    VentanaError("Fichero importado correctamente");
    getch();
    return;
}