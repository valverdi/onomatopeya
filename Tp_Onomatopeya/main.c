#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#ifdef WIN_32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include "api.h"
#include "lista-enlazada.h"
#include "Archivos.h"
#include "Maquina.h"
#include "Interfaz.h"
#include "Funciones.h"
#include "Estructuras.h"


void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* jugadoresLocales, int cantPartidas, char* Url, char* Token) ;
void mostrarRanking(t_lista* jugadoresAPI, SDL_Renderer* renderer, TTF_Font* font, char* Url, char* Token) ;
void salir(t_lista* jugadoresLocales,t_lista* jugadpresAPI);


int main() {
    int cantPartidas;
    int primeraEjecucion = 1;
    int estadoActual = 0;
    t_lista jugadoresLocales;
    t_lista jugadoresAPI;
    char Url[100];
    char key[7];

    leerArchivo(Url, key, &cantPartidas);
    crear_lista(&jugadoresLocales);
    crear_lista(&jugadoresAPI);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Error inicializando SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Menu Principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VENTANA_ANCHO, VENTANA_ALTO, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error creando ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/Sora-Bold.ttf", 15);
    if (!font) {
        printf("Error cargando la fuente: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Color colorTexto = {255, 255, 255, 255};

    int centroX = (VENTANA_ANCHO - BOTON_ANCHO) / 2;
    int inicioY = (VENTANA_ALTO - (3 * BOTON_ALTO + 2 * ESPACIADO)) / 2;

    SDL_Rect botonJugar   = {50, inicioY, BOTON_ANCHO, BOTON_ALTO};
    SDL_Rect botonRanking = {50, inicioY + BOTON_ALTO + ESPACIADO, BOTON_ANCHO, BOTON_ALTO};
    SDL_Rect botonSalir   = {50, inicioY + 2 * (BOTON_ALTO + ESPACIADO), BOTON_ANCHO, BOTON_ALTO};

    SDL_Event e;
    int salirDelPrograma = 0;

    while (!salirDelPrograma) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                salirDelPrograma = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                int x = e.button.x;
                int y = e.button.y;

                if (dentroDeBoton(x, y, botonJugar)) {
                    SDL_RenderPresent(renderer);
                    jugar(renderer, font, &jugadoresLocales, cantPartidas, Url, key);
                    estadoActual = 1;
                } else if (dentroDeBoton(x, y, botonRanking)) {
                    SDL_RenderPresent(renderer);
                    mostrarRanking(&jugadoresAPI, renderer, font, Url, key);
                    estadoActual = 2;
                } else if (dentroDeBoton(x, y, botonSalir)) {
                    SDL_RenderPresent(renderer);
                    salir(&jugadoresLocales,&jugadoresAPI);
                    salirDelPrograma = 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
        SDL_RenderFillRect(renderer, &botonJugar);
        SDL_RenderFillRect(renderer, &botonRanking);
        SDL_RenderFillRect(renderer, &botonSalir);

        renderizarTexto(renderer, "Jugar", font, colorTexto, &botonJugar);
        renderizarTexto(renderer, "Mostrar Ranking", font, colorTexto, &botonRanking);
        renderizarTexto(renderer, "Salir", font, colorTexto, &botonSalir);

        if(primeraEjecucion == 1)
        {
            SDL_RenderPresent(renderer);
            primeraEjecucion = 0;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void salir(t_lista* jugadoresLocales,t_lista* jugadoresAPI) {
    if(!lista_vacia(jugadoresLocales))
        vaciar_lista(jugadoresLocales);
    if(!lista_vacia(jugadoresAPI))
        vaciar_lista(jugadoresAPI);

    printf("Saliendo...\n");
}

void mostrarRanking(t_lista* jugadoresAPI, SDL_Renderer* renderer, TTF_Font* font, char* Url, char* Token) {
    printf("Mostrando Ranking...\n");

    SDL_Color colorTexto = {255, 255, 255, 255};
    int x = 250;
    int y = 50;

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    // Limpiar y volver a llenar la lista desde la API
    vaciar_lista(jugadoresAPI);

    obtener_jugadores(Url, Token, jugadoresAPI, 100);

    if (lista_vacia(jugadoresAPI)) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, "No hay jugadores en el ranking.", colorTexto);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destino = {x, y, surface->w, surface->h};

        SDL_RenderCopy(renderer, texture, NULL, &destino);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        SDL_RenderPresent(renderer);
        return;
    }

    // Título "RANKING:"
    SDL_Surface* surfaceTitulo = TTF_RenderText_Solid(font, "RANKING:", colorTexto);
    SDL_Texture* textureTitulo = SDL_CreateTextureFromSurface(renderer, surfaceTitulo);
    SDL_Rect destinoTitulo = {x, y, surfaceTitulo->w, surfaceTitulo->h};

    SDL_RenderCopy(renderer, textureTitulo, NULL, &destinoTitulo);
    SDL_DestroyTexture(textureTitulo);
    SDL_FreeSurface(surfaceTitulo);

    y += 50;

    // Pasar renderer, font, y extra
    void* datosExtra[] = { renderer, font, &y };
    map_lista(jugadoresAPI, renderizarJugadorEnRanking, datosExtra);

    SDL_RenderPresent(renderer);
}

void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* jugadoresLocales, int cantPartidas, char* Url, char* Token) {
    int cantidadJugadores = 0;

    SDL_StartTextInput();
    pedirCantidadJugadores(renderer, font, &cantidadJugadores);
    SDL_StopTextInput();

    if (cantidadJugadores <= 0) {
        printf("Cantidad inválida o cancelado.\n");
        return;
    }
    printf("Cantidad de jugadores: %d\n", cantidadJugadores);

    SDL_StartTextInput();
    pedirNombres(renderer, font, cantidadJugadores, jugadoresLocales);
    SDL_StopTextInput();

    SDL_StartTextInput();
    empezar_partida(renderer, font, cantidadJugadores, jugadoresLocales, cantPartidas, Url, Token);
    SDL_StopTextInput();
}
