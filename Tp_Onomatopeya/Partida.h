#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "lista-enlazada.h"
#include "Estructuras.h"
#include "Interfaz.h"
#include "Maquina.h"

void jugarPartida(SDL_Renderer* renderer, TTF_Font* font, Jugador* jugador, t_lista* lr);

void inicializarTablero(Tablero* t);
char hayGanador(Tablero* t);
int tableroLleno(Tablero* t);

#endif // PARTIDA_H_INCLUDED
