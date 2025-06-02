#ifndef API_H
#define API_H



/**
 * Env�a una lista de jugadores en formato JSON a una URL usando cURL.
 *
 * @param url           La URL a la que se enviar� el POST.
 * @param codigoGrupo   El valor de "CodigoGrupo" que se incluir� en el JSON.
 * @param jugadores     Un arreglo de estructuras Jugador.
 * @param cantidad      La cantidad de jugadores en el arreglo.
 */
void enviar_jugadores_con_curl(const char* url, const char* codigoGrupo, Jugador* jugadores, int cantidad);

#endif // API_H
