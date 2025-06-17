#include "Funciones.h"

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores) {

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    // Texto de pregunta
    SDL_Rect pregunta = {(areaDerecha.x + (areaDerecha.w) / 2), 100, 0, 0};
    renderizarTexto(renderer, "Cuantos jugadores?", font, colorTexto, &pregunta);

    // Mostrar lo que llevamos hasta ahora
    SDL_RenderPresent(renderer);

    // Capturar entrada
    int salirCaptura = 0;
    char entrada[3] = {0}; // Para 2 dígitos como máximo
    int indice = 0;

    while (!salirCaptura) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                *cantidadJugadores = -1;
                return;
            }
            if (e.type == SDL_TEXTINPUT) {
                if (indice < 2) { // No más de 2 caracteres
                    entrada[indice] = e.text.text[0];
                    indice++;
                    entrada[indice] = '\0';
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                    indice--;
                    entrada[indice] = '\0';
                }
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    salirCaptura = 1;
                }
            }
        }

        // Redibujar cada vez que cambia
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        renderizarTexto(renderer, "Cuantos jugadores?", font, colorTexto, &pregunta);

        if(strlen(entrada) > 0) {
            SDL_Rect respuesta = {(areaDerecha.x + (areaDerecha.w) / 2), 200, 0, 0};
            renderizarTexto(renderer, entrada, font, colorTexto, &respuesta);
        }

        SDL_RenderPresent(renderer);
    }

    *cantidadJugadores = atoi(entrada); // Convertir entrada a número
}

void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p) {
    fflush(stdin);

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};

    int i;

    for (i = 0; i < cantidadJugadores; i++) {

        // Limpiar el área derecha antes de dibujar
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        // Texto de pregunta
        char pregunta[50];
        sprintf(pregunta, "Jugador %d, ingrese nombre:", i + 1);

        // Obtener el tamaño del texto de la pregunta para centrarlo
        SDL_Surface* surfacePregunta = TTF_RenderText_Solid(font, pregunta, colorTexto);
        SDL_Rect preguntaRect = {areaDerecha.x + (areaDerecha.w - surfacePregunta->w) / 2, 100, surfacePregunta->w, surfacePregunta->h};
        renderizarTexto(renderer, pregunta, font, colorTexto, &preguntaRect);
        SDL_FreeSurface(surfacePregunta);

        // Mostrar lo que llevamos hasta ahora
        SDL_RenderPresent(renderer);

        // Capturar entrada
        int salirCaptura = 0;
        char entrada[50] = {0}; // Nombres de 50 caracteres max
        int indice = 0;

        while (!salirCaptura) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    return;
                }
                if (e.type == SDL_TEXTINPUT) {
                    if (indice < 50) {
                        entrada[indice] = e.text.text[0];
                        indice++;
                        entrada[indice] = '\0';
                    }
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                        indice--;
                        entrada[indice] = '\0';
                    }
                    if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                        salirCaptura = 1;
                    }
                }
            }

            // Limpiar el área derecha antes de redibujar
            SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
            SDL_RenderFillRect(renderer, &areaDerecha);

            // Redibujar la pregunta centrada
            surfacePregunta = TTF_RenderText_Solid(font, pregunta, colorTexto);
            preguntaRect.x = areaDerecha.x + (areaDerecha.w - surfacePregunta->w) / 2;
            preguntaRect.y = 100;
            renderizarTexto(renderer, pregunta, font, colorTexto, &preguntaRect);
            SDL_FreeSurface(surfacePregunta);

            // Mostrar el texto que lleva el usuario hasta ahora (nombre ingresado)
            int anchoTexto = 0;
            int altoTexto = 0;
            if(strlen(entrada) > 0) {
                TTF_SizeText(font,entrada,&anchoTexto,&altoTexto);
                SDL_Rect respuestaRect = {areaDerecha.x + (areaDerecha.w - anchoTexto) / 2, 200, anchoTexto, altoTexto};
                renderizarTexto(renderer, entrada, font, colorTexto, &respuestaRect);
            }

            // Presentar la actualización de la pantalla
            SDL_RenderPresent(renderer);
        }

        // Guardar el nombre en la lista
        Jugador ingreso;
        memset(&ingreso,0,sizeof(Jugador));
        strncpy(ingreso.nombre,entrada,(sizeof(ingreso.nombre)-1));
        ingreso.nombre[sizeof(ingreso.nombre)-1] = '\0';
        printf("\nNombre guardado: %s\n",ingreso.nombre);
        ingreso.puntaje = 0;

        poner_al_comienzo_lista(p, &ingreso, sizeof(Jugador));
    }
}

void empezar_partida(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p, int cantPartidas, char* Url, char* Token){

    SDL_Color colorTexto = {255, 255, 255, 255};

    Jugador JugadorActual;
    memset(&JugadorActual,0,sizeof(JugadorActual));

    t_lista listaResultados;
    crear_lista(&listaResultados);

    int elementosRestantes = contar_elementos_lista(p);;
    int numeroAleatorio;
    srand((unsigned int)time(NULL));

    while(!lista_vacia(p) || elementosRestantes > 0)
    {
        //elegir random
        if(elementosRestantes <= 0) {
            printf("No hay elementos en la lista.\n");
            break;
        }
        numeroAleatorio = rand() % elementosRestantes;

        if(numeroAleatorio < 0 || numeroAleatorio >= elementosRestantes) {
            printf("Posición inválida: %d\n", numeroAleatorio);
            break;
        }

        //sacarlo de la lista
        if(sacar_de_pos_lista(p,&JugadorActual,sizeof(JugadorActual),numeroAleatorio))
        {
            printf("Error al sacar jugador de lista.\n");
            break;
        }

        printf("Jugando: %s\n", JugadorActual.nombre);

        //jugar la partida
        for (int i = 0; i<cantPartidas; i++)
        {
            jugarPartida(renderer, font, &JugadorActual, &listaResultados);
        }
        enviar_jugadores_con_curl(Url, Token, &JugadorActual, 1);
        poner_al_final_lista(p,&JugadorActual,sizeof(JugadorActual));
        elementosRestantes --;
    }

    escribirTXTResultados(&listaResultados, p);

    vaciar_lista(&listaResultados);
    vaciar_lista(p);

    #ifdef WIN_32
        Sleep(1000);
    #else
        sleep(1);
    #endif

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);
    SDL_RenderPresent(renderer);
}

void imprimirJugador(const void* dato) {
    const Jugador* jugador = (const Jugador*)dato;
    printf("- %s (Puntaje: %d)\n", jugador->nombre, jugador->puntaje);
}

