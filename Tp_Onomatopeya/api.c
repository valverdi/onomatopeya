#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"

typedef struct {
    const char* nombre;
    int puntos;
} Jugador;

void enviar_jugadores_con_curl(const char* url, const char* codigoGrupo, Jugador* jugadores, int cantidad);
int main() {
    Jugador jugadores[] = {
        {"pedro", 10},
        {"Juan", 2}
    };
    enviar_jugadores_con_curl("https://algoritmos-api.azurewebsites.net/api/TaCTi/", "buffer", jugadores, 2);
    return 0;
}

void enviar_jugadores_con_curl(const char* url, const char* codigoGrupo, Jugador* jugadores, int cantidad) {
    // Armar el JSON manualmente
    char json[2048];
    strcpy(json, "{\n  \"CodigoGrupo\": \"");
    strcat(json, codigoGrupo);
    strcat(json, "\",\n  \"Jugadores\": [\n");

    for (int i = 0; i < cantidad; i++) {
        char jugadorStr[256];
        sprintf(jugadorStr,
            "    {\n"
            "      \"nombre\": \"%s\",\n"
            "      \"puntos\": %d\n"
            "    }%s\n",
            jugadores[i].nombre,
            jugadores[i].puntos,
            (i < cantidad - 1) ? "," : ""
        );
        strcat(json, jugadorStr);
    }

    strcat(json, "  ]\n}");

    // Mostrar el JSON
    printf("JSON a enviar:\n%s\n\n", json);

    // Inicializar libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "No se pudo inicializar CURL.\n");
        return;
    }

    CURLcode res;

    // Cabecera Content-Type
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configurar CURL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Hacer la petición
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error al hacer POST: %s\n", curl_easy_strerror(res));
    }

    // Limpiar
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}



