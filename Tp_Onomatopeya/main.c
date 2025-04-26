#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "lista-enlazada.h"

// Macros para tamaño de ventana
#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

// Macros para tamaño de botón
#define BOTON_ANCHO 400
#define BOTON_ALTO 100
#define ESPACIADO 30


void jugar();
void mostrarRanking();
void salir();

int dentroDeBoton(int x, int y, SDL_Rect boton);

void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton);


int main() {

    t_lista p;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Error inicializando SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Menú Principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VENTANA_ANCHO, VENTANA_ALTO, SDL_WINDOW_SHOWN);
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

    TTF_Font* font = TTF_OpenFont("assets/Sora-Bold.ttf", 28);
    if (!font) {
        printf("Error cargando la fuente: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color colorTexto = {255, 255, 255, 255};

    int centroX = (VENTANA_ANCHO - BOTON_ANCHO) / 2;
    int inicioY = (VENTANA_ALTO - (3 * BOTON_ALTO + 2 * ESPACIADO)) / 2;

    SDL_Rect botonJugar   = {centroX, inicioY, BOTON_ANCHO, BOTON_ALTO};
    SDL_Rect botonRanking = {centroX, inicioY + BOTON_ALTO + ESPACIADO, BOTON_ANCHO, BOTON_ALTO};
    SDL_Rect botonSalir   = {centroX, inicioY + 2 * (BOTON_ALTO + ESPACIADO), BOTON_ANCHO, BOTON_ALTO};

    SDL_Event e;
    int salirDelPrograma = 0;

    while (!salirDelPrograma) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                salirDelPrograma = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (dentroDeBoton(x, y, botonJugar)) {
                    jugar(&p);
                } else if (dentroDeBoton(x, y, botonRanking)) {
                    mostrarRanking(&p);
                } else if (dentroDeBoton(x, y, botonSalir)) {
                    salir(&p);
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

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

int dentroDeBoton(int x, int y, SDL_Rect boton) {
    return (x >= boton.x && x <= boton.x + boton.w && y >= boton.y && y <= boton.y + boton.h);
}

void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, texto, color);
    if (!surface) {
        printf("Error al crear la superficie del texto: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Error al crear la textura del texto: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destino;
    destino.w = surface->w;
    destino.h = surface->h;
    destino.x = boton->x + (boton->w - destino.w) / 2;
    destino.y = boton->y + (boton->h - destino.h) / 2;

    SDL_RenderCopy(renderer, texture, NULL, &destino);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void salir(t_lista* p) {
    if(!lista_vacia(p))
        vaciar_lista(p);

    printf("Saliendo\n");

}

void mostrarRanking(t_lista* p) {
    if(!lista_vacia(p))
    {
        printf("Mostrando Ranking\n");
    ///Hay que hacer la funcion de printear una estructura de un String|int
        //mostrar_lista(p);
    }
    else
    {
        printf("Intenta mostrar lista vacia");
    }
}

void jugar(t_lista* p) {
    printf("Iniciando Partida\n");
    ///pedir jugadores

    ///elegir jugador

    ///llamar a funcion de biblioteca aparte

    ///siguiente jugador
}
