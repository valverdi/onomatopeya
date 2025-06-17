#include "Archivos.h"

void leerArchivo(char* url, char* key, int* cantPartidas){
    FILE* archivo = fopen("config.txt", "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return;
    }

    // Leer URL y key
    fscanf(archivo, " %99[^|]| %99[^\n]", url, key);

    // Leer número entero en la segunda línea
    if(fscanf(archivo, "%d", cantPartidas) != 1)
    {
        perror("Error al leer cantidad de partidas.\n");
        *cantPartidas = 0;
    }

    fclose(archivo);
}

void escribirTXTResultados(t_lista* lr, t_lista* p){

    Resultado resultado;
    Jugador jugador;
    int maxPuntaje = 0;
    int bandMax = 0;
    t_lista listaMaxPuntaje;
    crear_lista(&listaMaxPuntaje);

    char nombreArchivo[100];
    generarNombreArchivo(nombreArchivo, sizeof(nombreArchivo));

    FILE* archivo = fopen(nombreArchivo, "a");
    if (!archivo) {
        perror("No se pudo abrir el archivo");
        return;
    }

    const char* ganador;

    printf("Generando Log...\n");

    while(!lista_vacia(lr))
    {
        sacar_primero_lista(lr, &resultado, sizeof(Resultado));

        fprintf(archivo, "-----------------------------\n");
        fprintf(archivo, "JUGADOR: %s\n", resultado.jugador.nombre);
        fprintf(archivo, "-TABLERO:\n");

        for (int i = 0; i < 3; i++) {
            fprintf(archivo, "         "); // sangría para que quede alineado
            for (int j = 0; j < 3; j++) {
                fprintf(archivo, "%c ", resultado.tablero.matriz[i][j]);
            }
            fprintf(archivo, "\n");
        }

         ganador = obtenerGanador(resultado.puntaje);

        fprintf(archivo, "-GANADOR : \"%s\"\n", ganador);
        fprintf(archivo, "-PUNTAJE: %d\n", resultado.puntaje);
        fprintf(archivo, "-PUNTAJE TOTAL DE JUGADOR: %d\n\n\n", resultado.jugador.puntaje);
    }

    fprintf(archivo, "-----------------------------\n");
    fprintf(archivo, "-----------------------------\n");
    fprintf(archivo, "\nPuntaje Final de la Tanda\n");
    while(!lista_vacia(p))
    {
        sacar_primero_lista(p, &jugador, sizeof(Jugador));
        fprintf(archivo, "- %s : %d\n", jugador.nombre, jugador.puntaje);

        if(bandMax == 0)
        {
            bandMax = 1;
            maxPuntaje = jugador.puntaje;
            poner_al_comienzo_lista(&listaMaxPuntaje, &jugador, sizeof(Jugador));
        }
        else if(maxPuntaje == jugador.puntaje)
        {
            poner_al_comienzo_lista(&listaMaxPuntaje, &jugador, sizeof(Jugador));
        }
        else if(maxPuntaje < jugador.puntaje)
        {
            vaciar_lista(&listaMaxPuntaje);
            maxPuntaje = jugador.puntaje;
            poner_al_comienzo_lista(&listaMaxPuntaje, &jugador, sizeof(Jugador));
        }
    }

    fprintf(archivo, "-----------------------------\n");
    fprintf(archivo, "-----------------------------\n");
    fprintf(archivo, "\nJugador/es con Mayor Puntaje\n");
    while(!lista_vacia(&listaMaxPuntaje))
    {
        sacar_primero_lista(&listaMaxPuntaje, &jugador, sizeof(Jugador));
        fprintf(archivo, "- %s : %d\n", jugador.nombre, jugador.puntaje);
    }

    vaciar_lista(&listaMaxPuntaje);
    fclose(archivo);
}

const char* obtenerGanador(int puntaje) {
    switch (puntaje) {
        case 3: return "JUGADOR";
        case 2: return "EMPATE";
        case -1: return "MAQUINA";
        default: return "DESCONOCIDO";
    }
}

void generarNombreArchivo(char* nombreArchivo, size_t tam) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    strftime(nombreArchivo, tam, "informe-juego_%Y-%m-%d-%H-%M.txt", tm_info);
}
