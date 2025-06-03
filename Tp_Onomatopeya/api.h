#ifndef API_H
#define API_H

#include "macros.h"
#include "lista-enlazada.h"
#include "cJson.h"

void enviar_jugadores_con_curl(const char* url, const char* codigoGrupo, Jugador* jugadores, int cantidad);
int obtener_jugadores(const char* base_url, const char* password, t_lista* lista , int max_jugadores);
size_t escribir_callback(void* contenido, size_t size, size_t nmemb, void* userp);

#endif // API_H
