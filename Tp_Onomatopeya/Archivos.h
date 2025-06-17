#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <time.h>

#include "lista-enlazada.h"
#include "Estructuras.h"

void leerArchivo(char* url, char* key, int* cantPartidas);

void escribirTXTResultados(t_lista* lr, t_lista* p);
const char* obtenerGanador(int puntaje);
void generarNombreArchivo(char* nombreArchivo, size_t tam);

#endif // ARCHIVOS_H_INCLUDED
