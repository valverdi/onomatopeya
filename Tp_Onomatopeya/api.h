#ifndef API_H
#define API_H

#include "macros.h"

void enviar_jugadores_con_curl(const char* url, const char* codigoGrupo, Jugador* jugadores, int cantidad);

#endif // API_H
