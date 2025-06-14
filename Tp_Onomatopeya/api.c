#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"
#define MAX_JUGADORES 100
#define TAM_BUFFER_RESPUESTA 8192


typedef struct {
    char buffer[TAM_BUFFER_RESPUESTA];
    size_t size;
} ResponseData;

size_t escribir_callback(void* contenido, size_t size, size_t nmemb, void* userp) {
    ResponseData* resp = (ResponseData*)userp;
    size_t total = size * nmemb;

    if (resp->size + total >= TAM_BUFFER_RESPUESTA - 1)
        total = TAM_BUFFER_RESPUESTA - 1 - resp->size;

    memcpy(resp->buffer + resp->size, contenido, total);
    resp->size += total;
    resp->buffer[resp->size] = '\0';

    return total;
}

int cmpJugadores(const void* a, const void* b)
{
    const Jugador* jugadorA = (const Jugador*)a;
    const Jugador* jugadorB = (const Jugador*)b;

    return (jugadorB->puntaje - jugadorA->puntaje);
}

int obtener_jugadores(const char* base_url, const char* password, t_lista * lista, int max_jugadores) {
    char url[512];
    snprintf(url, sizeof(url), "%s/%s", base_url, password);

    Jugador jugador;

    CURL* curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "No se pudo inicializar CURL.\n");
        return 0;
    }

    ResponseData response = {.size = 0};
    response.buffer[0] = '\0';

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribir_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Error al hacer GET: %s\n", curl_easy_strerror(res));
        return 0;
    }

    cJSON* root = cJSON_Parse(response.buffer);
    if (!root || !cJSON_IsArray(root)) {
        fprintf(stderr, "Error al parsear JSON o el JSON no es un arreglo.\n");
        if (root) cJSON_Delete(root);
        return 0;
    }

    int count = 0;
    int total = cJSON_GetArraySize(root);

    for (int i = 0; i < total && count < max_jugadores; i++) {
        cJSON* item = cJSON_GetArrayItem(root, i);
        cJSON* nombre = cJSON_GetObjectItem(item, "nombreJugador");
        cJSON* puntaje = cJSON_GetObjectItem(item, "puntaje");

        if (nombre && puntaje && cJSON_IsString(nombre) && cJSON_IsNumber(puntaje)) {
            strncpy(jugador.nombre, nombre->valuestring, sizeof(jugador.nombre) - 1);
            jugador.nombre[sizeof(jugador.nombre) - 1] = '\0';
            jugador.puntaje = puntaje->valueint;

            poner_ordenado_lista(lista, &jugador, sizeof(Jugador), &cmpJugadores);
            count++;
        }
    }

    cJSON_Delete(root);
    return count;
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
            jugadores[i].puntaje,
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