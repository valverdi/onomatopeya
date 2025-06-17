#include "Partida.h"

void jugarPartida(SDL_Renderer* renderer, TTF_Font* font, Jugador* jugador, t_lista* lr){
    Tablero tablero;
    inicializarTablero(&tablero);

    char fichaJugador;
    char fichaMaquina;

    int turnoJugador = 0; // Empieza el jugador
    int jugando = 1;

    Resultado resultado;

    turnoJugador = rand() % 2;

    if(turnoJugador)
    {
        fichaJugador = 'X';
        fichaMaquina = 'O';
    }
    else
    {
        fichaJugador = 'O';
        fichaMaquina = 'X';
    }

    SDL_Color colorTexto = {255, 255, 255, 255};
    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};

    while (jugando)
    {
        // Limpiar solo el área derecha
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        // Dibujar el tablero
        dibujarTablero(renderer, font, &tablero);
        renderizarJugador(renderer, font, jugador, colorTexto, 450, 10);

        SDL_RenderPresent(renderer);

        SDL_Event e;
        SDL_WaitEvent(&e);
        if (e.type == SDL_QUIT)
        {
            jugando = 0;
            break;
        }

        if (turnoJugador && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && jugando)
        {
            int x = e.button.x;
            int y = e.button.y;

            if (clickEnTablero(&tablero, x, y, fichaJugador)) // jugador juega con 'X'
            {
                if (hayGanador(&tablero) == fichaJugador) {
                    jugador->puntaje += GANA; // GANÓ, sumar puntaje
                    jugando = 0;

                    resultado.tablero = tablero;
                    resultado.jugador = *jugador;
                    resultado.puntaje = GANA;

                    poner_al_final_lista(lr, &resultado, sizeof(Resultado));
                }
                else if (tableroLleno(&tablero)) {
                    jugador->puntaje += EMPATA;
                    jugando = 0; //EMPATE

                    resultado.tablero = tablero;
                    resultado.jugador = *jugador;
                    resultado.puntaje = EMPATA;

                    poner_al_final_lista(lr, &resultado, sizeof(Resultado));
                }
                else {
                    turnoJugador = 0; // Pasa turno a la máquina
                }
            }
        }
        else if (!turnoJugador && jugando)
        {
            // Turno de la máquina
            maquinaJuega(&tablero, fichaMaquina, fichaJugador);

            if (hayGanador(&tablero) == fichaMaquina) {
                jugador->puntaje += PIERDE;
                jugando = 0; //PERDIÓ

                resultado.tablero = tablero;
                resultado.jugador = *jugador;
                resultado.puntaje = PIERDE;

                poner_al_final_lista(lr, &resultado, sizeof(Resultado));
            }
            if (tableroLleno(&tablero)) {
                jugador->puntaje += EMPATA; // EMPATE
                jugando = 0;

                resultado.tablero = tablero;
                resultado.jugador = *jugador;
                resultado.puntaje = EMPATA;

                poner_al_final_lista(lr, &resultado, sizeof(Resultado));
            }
            turnoJugador = 1; // Vuelve a ser el turno del jugador
        }
        if(!jugando)
        {
            SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
            SDL_RenderFillRect(renderer, &areaDerecha);

            // Dibujar el tablero
            dibujarTablero(renderer, font, &tablero);
            renderizarJugador(renderer, font, jugador, colorTexto, 450, 10);

            SDL_RenderPresent(renderer);
        }
    }
}

void inicializarTablero(Tablero* t){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            t->matriz[i][j] = ' '; // Espacio vacío
        }
    }
}

char hayGanador(Tablero* t){
    // Revisar filas
    int i,j;

    for (i = 0; i < 3; i++)
    {
        if (t->matriz[i][0] != ' ' && t->matriz[i][0] == t->matriz[i][1] && t->matriz[i][1] == t->matriz[i][2])
            return t->matriz[i][0];
    }

    // Revisar columnas
    for (j = 0; j < 3; j++)
    {
        if (t->matriz[0][j] != ' ' && t->matriz[0][j] == t->matriz[1][j] && t->matriz[1][j] == t->matriz[2][j])
            return t->matriz[0][j];
    }

    // Revisar diagonales
    if (t->matriz[0][0] != ' ' && t->matriz[0][0] == t->matriz[1][1] && t->matriz[1][1] == t->matriz[2][2])
        return t->matriz[0][0];

    if (t->matriz[0][2] != ' ' && t->matriz[0][2] == t->matriz[1][1] && t->matriz[1][1] == t->matriz[2][0])
        return t->matriz[0][2];

    // No hay ganador
    return ' ';
}

int tableroLleno(Tablero* t){
    int i,j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (t->matriz[i][j] == ' ')
                return 0; // Hay al menos un espacio libre
        }
    }
    return 1; // Todo lleno
}

