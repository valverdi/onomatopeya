#ifndef LISTA_ENLAZADA_H_INCLUDED
#define LISTA_ENLAZADA_H_INCLUDED
#include "macros.h"

typedef Nodo* t_lista;

void crear_lista(t_lista* pl);

int lista_vacia(const t_lista* pl);
int lista_llena(const t_lista* pl, size_t tamElem);

void vaciar_lista(t_lista* pl);

int poner_al_comienzo_lista(t_lista* pl, const void* dato, size_t tamElem);
int poner_al_final_lista(t_lista* pl, const void* dato, size_t tamElem);
int poner_ordenado_lista(t_lista* pl, const void* dato, size_t tamElem, int(*cmp)(const void*, const void*));

int sacar_primero_lista(t_lista* pl, void* dato, size_t tamElem);
int sacar_ultimo_lista(t_lista* pl, void* dato, size_t tamElem);
int contar_elementos_lista(t_lista* pl);
int sacar_de_pos_lista(t_lista* pl, void* dato, size_t tamElem, unsigned int pos);


void mostrar_lista(t_lista* pl, void(*prnt)(const void* dato));
int ver_primero_lista(const t_lista* pl, void* dato, size_t tamElem);

//map-filter-reduce
void map_lista(const t_lista* pl, void(*accion)(void*, void*), void* extra);

Nodo* buscar_nodo_menor(Nodo* ini, int(*cmp)(const void*, const void*));
Nodo* sacar_nodo_menor(t_lista* pl, int(*cmp)(const void*, const void*));
int ordenar_lista_seleccion(t_lista* pl, int(*cmp)(const void*, const void*));

Nodo* buscarXclave(t_lista* pl, void* clave, size_t tamClave, int(*cmp)(const void*, const void*));




#endif // LISTA_ENLAZADA_H_INCLUDED
