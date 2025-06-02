#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define TAM_VEC 512
#define TAM_COLA 512

#define OK 0

#define PILA_LLENA -1
#define PILA_VACIA -2

#define COLA_LLENA -1
#define COLA_VACIA -2

#define VEC_LLENO -1

#define LISTA_LLENA -1
#define LISTA_VACIA -2

#define ARBOL_VACIO -2

#define ERR_ARCH -3
#define ERR_MEM -4

#define NO_EXISTE -912;

#define DUPLICADO 222;

#define MENOR(x,y)  (((x) > (y) ) ? (y) : (x) )
#define MAYOR(x,y) (((x) > (y) ) ? (x) : (y) )
#define TOUPPER(X) ( ( (X) > 96 ) && ( (X) < 122 ) ? ( (X) - 32 ): (X))

#define RESERVAR_MEM(X,Y,Z,W) (((X) = (typeof(X)) malloc(Y)) == NULL ||((Z) = malloc(W)) == NULL ? free(X), 0 : 1 )


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Nodo lista simple

typedef struct sNodo
{
    void* elem;
    size_t tamElem;
    struct sNodo* nodoSig;

}Nodo;

//Nodo lista doble

typedef struct sNodoD
{
    void* elem;
    size_t tamElem;
    struct sNodoD* ant;
    struct sNodoD* sig;

}NodoD;

//Nodo arbol

typedef struct sNodoA
{
    void* info;
    size_t tam;
    struct sNodoA* izq;
    struct sNodoA* der;

}NodoA;


typedef struct {
    char nombre[50];
    int puntaje;
} Jugador;



int cmpInt(const void* dato1, const void* dato2);
void prntInt(const void* dato);
void accionPrntInt(void* dato, void* extra);



#endif // MACROS_H_INCLUDED
