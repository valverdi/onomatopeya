#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

// Funciones que se llaman cuando se hace clic en las opciones
void jugar() {
    printf("Has seleccionado Jugar\n");
}

void mostrarRanking() {
    printf("Has seleccionado Mostrar Ranking\n");
}

void salir() {
    printf("Has seleccionado Salir\n");
}

// Función para detectar si el clic está dentro de un botón
int dentroDeBoton(int x, int y, SDL_Rect boton) {
    return (x >= boton.x && x <= boton.x + boton.w && y >= boton.y && y <= boton.y + boton.h);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Menú Principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
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

    SDL_Rect botonJugar = {100, 100, 200, 50};
    SDL_Rect botonRanking = {100, 200, 200, 50};
    SDL_Rect botonSalir = {100, 300, 200, 50};

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
                    jugar();
                } else if (dentroDeBoton(x, y, botonRanking)) {
                    mostrarRanking();
                } else if (dentroDeBoton(x, y, botonSalir)) {
                    salir();
                    salirDelPrograma = 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 0);
        SDL_RenderClear(renderer);

        // Dibujar los botones
        SDL_SetRenderDrawColor(renderer, 23, 23, 23, 0);
        SDL_RenderFillRect(renderer, &botonJugar);
        SDL_RenderFillRect(renderer, &botonRanking);
        SDL_RenderFillRect(renderer, &botonSalir);

        // Mostrar el texto en los botones (simulado, ya que SDL2 no tiene función de texto directa)
        // Aquí deberías usar SDL_ttf para mostrar texto real, pero por simplicidad lo dejamos como un rectángulo.

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
