#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <time.h>

#include "Estructuras.h"


void renderizarJugador(SDL_Renderer* renderer, TTF_Font* font, const Jugador* jugador, SDL_Color color, int x, int y);

void renderizarJugadorEnRanking(void* elem, void* extra);

void dibujarTablero(SDL_Renderer* renderer, TTF_Font* font, Tablero* t);
int clickEnTablero(Tablero* t, int x, int y, char simbolo);

int dentroDeBoton(int x, int y, SDL_Rect boton);
void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton);

#endif // INTERFAZ_H_INCLUDED
