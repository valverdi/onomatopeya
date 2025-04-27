#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "lista-enlazada.h"

// Macros para tamaño de ventana
#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

// Macros para tamaño de botón
#define BOTON_ANCHO 150
#define BOTON_ALTO 50
#define ESPACIADO 30

typedef struct {
    char nombre[50];
    int puntaje;
} Jugador;

void cargarJugadoresPrueba(t_lista* pl);

void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* p) ;
void mostrarRanking(t_lista* pl, SDL_Renderer* renderer, TTF_Font* font) ;
void salir();

void imprimirJugador(const void* dato);

void renderizarJugador(SDL_Renderer* renderer, TTF_Font* font, const Jugador* jugador, SDL_Color color, int x, int y);

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores);
void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p);

int dentroDeBoton(int x, int y, SDL_Rect boton);
void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton);


int main() {
    int primeraEjecucion = 1;
    int estadoActual = 0;
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

    TTF_Font* font = TTF_OpenFont("assets/Sora-Bold.ttf", 15);
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
                    jugar(renderer, font, &p);
                    estadoActual = 1;
                } else if (dentroDeBoton(x, y, botonRanking)) {
                    SDL_RenderPresent(renderer);
                    mostrarRanking(&p, renderer, font);
                    estadoActual = 2;
                } else if (dentroDeBoton(x, y, botonSalir)) {
                    SDL_RenderPresent(renderer);
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

int dentroDeBoton(int x, int y, SDL_Rect boton) {
    return (x >= boton.x && x <= boton.x + boton.w && y >= boton.y && y <= boton.y + boton.h);
}

void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, texto, color);
    if (!surface) {
        //printf("Error al crear la superficie del texto: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        //printf("Error al crear la textura del texto: %s\n", SDL_GetError());
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

void mostrarRanking(t_lista* pl, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color colorTexto = {255, 255, 255, 255};
    int x = 250;
    int y = 50;

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    ///cargarJugadoresPrueba(pl); //////////////////////////////////////////Funcion de prueba

    if (lista_vacia(pl)) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, "No hay jugadores en el ranking.", colorTexto);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destino = {x, y, surface->w, surface->h};

        SDL_RenderCopy(renderer, texture, NULL, &destino);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        SDL_RenderPresent(renderer);
        return;
    }

    SDL_Surface* surfaceTitulo = TTF_RenderText_Solid(font, "RANKING:", colorTexto);
    SDL_Texture* textureTitulo = SDL_CreateTextureFromSurface(renderer, surfaceTitulo);
    SDL_Rect destinoTitulo = {x, y, surfaceTitulo->w, surfaceTitulo->h};

    SDL_RenderCopy(renderer, textureTitulo, NULL, &destinoTitulo);

    SDL_DestroyTexture(textureTitulo);
    SDL_FreeSurface(surfaceTitulo);

    y += 50;

    void renderizarJugadorEnRanking(void* elem, void* extra) {
        // Extra contiene el renderer, el font y las coordenadas x, y
        SDL_Renderer* renderer = ((void**)extra)[0];
        TTF_Font* font = ((void**)extra)[1];
        int* y = ((int**)extra)[2];

        // Convertir elem a Jugador
        Jugador* jugador = (Jugador*)elem;

        // Llamar a la función de renderizado de jugador
        renderizarJugador(renderer, font, jugador, colorTexto, 250, *y);

        // Actualizar la posición y para el siguiente jugador
        *y += 40;
    }


    void* extra[3] = {renderer, font, &y};

    map_lista(pl, renderizarJugadorEnRanking, extra);


    SDL_RenderPresent(renderer);

    ///vaciar_lista(pl); //////////////////////////////////////////Funcion de prueba
}

void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* p) {
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
    pedirNombres(renderer, font, cantidadJugadores, p);
    SDL_StopTextInput();

    // seguir el flujo...
}

void imprimirJugador(const void* dato) {
    const Jugador* jugador = (const Jugador*)dato;
    printf("- %s (Puntaje: %d)\n", jugador->nombre, jugador->puntaje);
}

void renderizarJugador(SDL_Renderer* renderer, TTF_Font* font, const Jugador* jugador, SDL_Color color, int x, int y) {
    char texto[100];
    snprintf(texto, sizeof(texto), "- %s (Puntaje: %d)", jugador->nombre, jugador->puntaje);

    SDL_Surface* surface = TTF_RenderText_Solid(font, texto, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destino = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &destino);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores) {

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    // Texto de pregunta
    SDL_Rect pregunta = {(areaDerecha.x + (areaDerecha.w) / 2), 100, 0, 0};
    renderizarTexto(renderer, "¿Cuántos jugadores?", font, colorTexto, &pregunta);

    // Mostrar lo que llevamos hasta ahora
    SDL_RenderPresent(renderer);

    // Capturar entrada
    int salirCaptura = 0;
    char entrada[3] = {0}; // Para 2 dígitos como máximo
    int indice = 0;

    while (!salirCaptura) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                *cantidadJugadores = -1;
                return;
            }
            if (e.type == SDL_TEXTINPUT) {
                if (indice < 2) { // No más de 2 caracteres
                    entrada[indice++] = e.text.text[0];
                    entrada[indice] = '\0';
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                    entrada[--indice] = '\0';
                }
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    salirCaptura = 1;
                }
            }
        }

        // Redibujar cada vez que cambia
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        renderizarTexto(renderer, "¿Cuántos jugadores?", font, colorTexto, &pregunta);

        SDL_Rect respuesta = {(areaDerecha.x + (areaDerecha.w) / 2), 200, 0, 0};
        renderizarTexto(renderer, entrada, font, colorTexto, &respuesta);

        SDL_RenderPresent(renderer);
    }

    *cantidadJugadores = atoi(entrada); // Convertir entrada a número
}

void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p) {
    fflush(stdin);

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};

    for (int i = 0; i < cantidadJugadores; i++) {

        // Limpiar el área derecha antes de dibujar
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        // Texto de pregunta
        char pregunta[50];
        sprintf(pregunta, "Jugador %d, ingrese nombre:", i + 1);

        // Obtener el tamaño del texto de la pregunta para centrarlo
        SDL_Surface* surfacePregunta = TTF_RenderText_Solid(font, pregunta, colorTexto);
        SDL_Rect preguntaRect = {areaDerecha.x + (areaDerecha.w - surfacePregunta->w) / 2, 100, surfacePregunta->w, surfacePregunta->h};
        renderizarTexto(renderer, pregunta, font, colorTexto, &preguntaRect);
        SDL_FreeSurface(surfacePregunta);

        // Mostrar lo que llevamos hasta ahora
        SDL_RenderPresent(renderer);

        // Capturar entrada
        int salirCaptura = 0;
        char entrada[50] = {0}; // Nombres de 50 caracteres max
        int indice = 0;

        while (!salirCaptura) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    return;
                }
                if (e.type == SDL_TEXTINPUT) {
                    if (indice < 50) {
                        entrada[indice++] = e.text.text[0];
                        entrada[indice] = '\0';
                    }
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                        entrada[--indice] = '\0';
                    }
                    if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                        salirCaptura = 1;
                    }
                }
            }

            // Limpiar el área derecha antes de redibujar
            SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
            SDL_RenderFillRect(renderer, &areaDerecha);

            // Redibujar la pregunta centrada
            surfacePregunta = TTF_RenderText_Solid(font, pregunta, colorTexto);
            preguntaRect.x = areaDerecha.x + (areaDerecha.w - surfacePregunta->w) / 2;
            preguntaRect.y = 100;
            renderizarTexto(renderer, pregunta, font, colorTexto, &preguntaRect);
            SDL_FreeSurface(surfacePregunta);

            // Mostrar el texto que lleva el usuario hasta ahora (nombre ingresado)
            SDL_Rect respuestaRect = {areaDerecha.x + (areaDerecha.w - strlen(entrada) * 10) / 2, 200, 0, 0};
            renderizarTexto(renderer, entrada, font, colorTexto, &respuestaRect);

            // Presentar la actualización de la pantalla
            SDL_RenderPresent(renderer);
        }

        // Guardar el nombre en la lista
        Jugador ingreso;
        strncpy(ingreso.nombre, entrada, sizeof(ingreso.nombre) - 1);
        ingreso.puntaje = 0;

        poner_al_final_lista(p, &ingreso, sizeof(Jugador));

    }
}

void cargarJugadoresPrueba(t_lista* pl) {
    const char* nombres[] = {"Jugador 1", "Jugador 2", "Jugador 3"};
    int i;

    for (i = 0; i < 3; i++) {
        poner_al_final_lista(pl, nombres[i], strlen(nombres[i]) + 1);
    }
}

