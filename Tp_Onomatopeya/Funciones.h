#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "Estructuras.h"
#include "lista-enlazada.h"
#include "Interfaz.h"
#include "Partida.h"
#include "Archivos.h"
#include "api.h"

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores);
void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p);
void empezar_partida(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p, int cantPartidas, char* Url, char* Token);

void imprimirJugador(const void* dato);

#endif // FUNCIONES_H_INCLUDED
