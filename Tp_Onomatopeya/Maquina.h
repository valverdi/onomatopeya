#ifndef MAQUINA_H_INCLUDED
#define MAQUINA_H_INCLUDED
#include "Estructuras.h"

#define VACIO ' '

void maquinaJuega(Tablero* t, char fichaMaquina, char fichaJugador);
int evaluar(char tablero[3][3],char fichaJugador, char fichaMaquina);
int hayMovimientos(char tablero[3][3]);
int minimax(char tablero[3][3],int esMaximizador,char fichaJugador,char fichaMaquina);
void mejorJugada(char tablero[3][3],int *mejorFila,int *mejorCol,char fichaJugador,char fichaMaquina);

#endif // MAQUINA_H_INCLUDED
