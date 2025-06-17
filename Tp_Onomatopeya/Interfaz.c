#include "Interfaz.h"

void renderizarJugador(SDL_Renderer* renderer, TTF_Font* font, const Jugador* jugador, SDL_Color color, int x, int y) {
    char texto[100];
    snprintf(texto, sizeof(texto), "%s || Puntaje: %d", jugador->nombre, jugador->puntaje);

    SDL_Surface* surface = TTF_RenderText_Solid(font, texto, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destino = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &destino);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int dentroDeBoton(int x, int y, SDL_Rect boton) {
    return (x >= boton.x && x <= boton.x + boton.w && y >= boton.y && y <= boton.y + boton.h);
}

void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton) {
    if (texto == NULL || font == NULL) {
    printf("Error: el texto es NULL.\n");
    return;
    }

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

void dibujarTablero(SDL_Renderer* renderer, TTF_Font* font, Tablero* t) {
    char ficha;
    char texto[2];
    int fila;
    int col;

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Color ColorX = {100, 149, 237, 255}; // Azul claro (Cornflower Blue)
    SDL_Color ColorO = {220, 20, 60, 255};   // Rojo carmesí (Crimson)

    int xOffset = 250; // Desplazamiento desde el borde izquierdo
    int yOffset = 0;   // Empezar desde la parte superior de la zona derecha
    int anchoZonaDerecha = VENTANA_ANCHO - 250;
    int altoZonaDerecha = VENTANA_ALTO;

    TTF_Font* fuente = TTF_OpenFont("assets/Sora-Bold.ttf", 50);
    if(!fuente){
        printf("Error al cargar la fuente.\n");
        return;
    }

    // Calcular el tamaño de cada casilla basado en el tamaño del área disponible
    int size = (anchoZonaDerecha < altoZonaDerecha) ? anchoZonaDerecha / 3 : altoZonaDerecha / 3;

    // Dibujar las líneas del tablero (3x3)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanco
    for (int i = 1; i < 3; i++) {
        // Líneas horizontales
        SDL_RenderDrawLine(renderer, xOffset, yOffset + i * size, xOffset + 3 * size, yOffset + i * size);
        // Líneas verticales
        SDL_RenderDrawLine(renderer, xOffset + i * size, yOffset, xOffset + i * size, yOffset + 3 * size);
    }

    // Dibujar X y O en las casillas
    for (fila = 0; fila < 3; fila++) {
        for (col = 0; col < 3; col++) {
            ficha = t->matriz[fila][col];
            if (ficha != ' ') { // Solo dibujar si la casilla no está vacía
                texto[0] = ficha;
                texto[1] = '\0'; // Convertir la ficha a una cadena de texto
                if( ficha == 'X')
                {
                    SDL_Surface* surface = TTF_RenderText_Solid(fuente, texto, ColorX);
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

                    // Calcular la posición de la casilla
                    SDL_Rect destRect = {xOffset + col * size + (size - surface->w) / 2,
                                         yOffset + fila * size + (size - surface->h) / 2,
                                         surface->w, surface->h};

                    // Dibujar la ficha (X o O)
                    SDL_RenderCopy(renderer, texture, NULL, &destRect);

                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
                else if(ficha == 'O')
                {
                    SDL_Surface* surface = TTF_RenderText_Solid(fuente, texto, ColorO);
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

                    // Calcular la posición de la casilla
                    SDL_Rect destRect = {xOffset + col * size + (size - surface->w) / 2,
                                         yOffset + fila * size + (size - surface->h) / 2,
                                         surface->w, surface->h};

                    // Dibujar la ficha (X o O)
                    SDL_RenderCopy(renderer, texture, NULL, &destRect);

                    SDL_DestroyTexture(texture);
                    SDL_FreeSurface(surface);
                }
            }
        }
    }
    if(fuente)
        TTF_CloseFont(fuente);
}

int clickEnTablero(Tablero* t, int x, int y, char simbolo){
    int margenX = 250; // Porque dibujamos desde x = 250
    int anchoCelda = (VENTANA_ANCHO - margenX) / 3;
    int altoCelda = VENTANA_ALTO / 3;

    // Ajustar coordenadas
    x -= margenX;

    if (x < 0 || y < 0 || x >= (VENTANA_ANCHO - margenX) || y >= VENTANA_ALTO)
        return 0; // Click fuera del tablero

    int col = x / anchoCelda;
    int fil = y / altoCelda;

    if(fil >= 0 && fil < 3 && col >=0 && col < 3) {
        if (t->matriz[fil][col] == ' ')
        {
            t->matriz[fil][col] = simbolo;
            return 1; // Movimiento válido
        }
    }

    return 0; // Casilla ocupada
}

void renderizarJugadorEnRanking(void* elem, void* extra) {
    // Extra contiene: renderer, font, puntero a y
    void** datos = (void**)extra;

    SDL_Renderer* renderer = (SDL_Renderer*)datos[0];
    TTF_Font* font = (TTF_Font*)datos[1];
    int* y = (int*)datos[2];

    Jugador* jugador = (Jugador*)elem;

    SDL_Color colorTexto = {255, 255, 255, 255};
    renderizarJugador(renderer, font, jugador, colorTexto, 250, *y);

    *y += 40;  // Incrementar la posición vertical
}
