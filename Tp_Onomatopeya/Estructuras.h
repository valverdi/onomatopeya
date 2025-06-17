#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifdef WIN_32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include <stdio.h>
#include <time.h>

#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

// Macros para tama�o de bot�n
#define BOTON_ANCHO 150
#define BOTON_ALTO 50
#define ESPACIADO 30

#define GANA 3
#define PIERDE -1
#define EMPATA 2

#define VACIO ' '

typedef struct {
    char nombre[50];
    int puntaje;
} Jugador;

typedef struct {
    char matriz[3][3];
} Tablero;

typedef struct {
    Tablero tablero;
    Jugador jugador;
    int puntaje;
} Resultado;

#endif // ESTRUCTURAS_H_INCLUDED
