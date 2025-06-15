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

// Macros para tama�o de ventana
#define VENTANA_ANCHO 800
#define VENTANA_ALTO 600

// Macros para tama�o de bot�n
#define BOTON_ANCHO 150
#define BOTON_ALTO 50
#define ESPACIADO 30

typedef struct {
    char matriz[3][3];
} Tablero;

void leerArchivo(char* url, char* key, int* cantPartidas);

void cargarJugadoresPrueba(t_lista* pl);

void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* jugadoresLocales, int cantPartidas) ;
void mostrarRanking(t_lista* jugadoresAPI, SDL_Renderer* renderer, TTF_Font* font) ;
void salir(t_lista* jugadoresLocales,t_lista* jugadpresAPI);

void imprimirJugador(const void* dato);

void renderizarJugador(SDL_Renderer* renderer, TTF_Font* font, const Jugador* jugador, SDL_Color color, int x, int y);

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores);
void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p);
void empezar_partida(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p, int cantPartidas);
void jugarPartida(SDL_Renderer* renderer, TTF_Font* font, Jugador* jugadorActual);

void inicializarTablero(Tablero* t);
void dibujarTablero(SDL_Renderer* renderer, TTF_Font* font,     Tablero* t);
int clickEnTablero(Tablero* t, int x, int y, char simbolo);
void maquinaJuega(Tablero* t, char ficha);
char hayGanador(Tablero* t);
int tableroLleno(Tablero* t);

int dentroDeBoton(int x, int y, SDL_Rect boton);
void renderizarTexto(SDL_Renderer* renderer, const char* texto, TTF_Font* font, SDL_Color color, SDL_Rect* boton);


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
                    jugar(renderer, font, &jugadoresLocales, cantPartidas);
                    estadoActual = 1;
                } else if (dentroDeBoton(x, y, botonRanking)) {
                    SDL_RenderPresent(renderer);
                    mostrarRanking(&jugadoresAPI, renderer, font);
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

void salir(t_lista* jugadoresLocales,t_lista* jugadoresAPI) {
    if(!lista_vacia(jugadoresLocales))
        vaciar_lista(jugadoresLocales);
    if(!lista_vacia(jugadoresAPI))
        vaciar_lista(jugadoresAPI);

    printf("Saliendo...\n");
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

void mostrarRanking(t_lista* jugadoresAPI, SDL_Renderer* renderer, TTF_Font* font) {
    printf("Mostrando Ranking...\n");

    SDL_Color colorTexto = {255, 255, 255, 255};
    int x = 250;
    int y = 50;

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    // Limpiar y volver a llenar la lista desde la API
    vaciar_lista(jugadoresAPI);

    obtener_jugadores("https://algoritmos-api.azurewebsites.net/api/TaCTi", "buffer", jugadoresAPI, 100);

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

void jugar(SDL_Renderer* renderer, TTF_Font* font, t_lista* jugadoresLocales, int cantPartidas) {
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
    empezar_partida(renderer, font, cantidadJugadores, jugadoresLocales, cantPartidas);
    SDL_StopTextInput();
}

void empezar_partida(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p, int cantPartidas){

    SDL_Color colorTexto = {255, 255, 255, 255};

    Jugador JugadorActual;
    memset(&JugadorActual,0,sizeof(JugadorActual));

    int elementosRestantes = contar_elementos_lista(p);;
    int numeroAleatorio;
    srand((unsigned int)time(NULL));

    while(!lista_vacia(p))
    {
        //elegir random
        if(elementosRestantes <= 0) {
            printf("No hay elementos en la lista.\n");
            break;
        }
        numeroAleatorio = rand() % elementosRestantes;

        if(numeroAleatorio < 0 || numeroAleatorio >= elementosRestantes) {
            printf("Posición inválida: %d\n", numeroAleatorio);
            break;
        }

        //sacarlo de la lista
        if(sacar_de_pos_lista(p,&JugadorActual,sizeof(JugadorActual),numeroAleatorio))
        {
            printf("Error al sacar jugador de lista.\n");
            break;
        }

        printf("Jugando: %s\n", JugadorActual.nombre);

        //jugar la partida
        for (int i = 0; i<cantPartidas; i++)
        {
            jugarPartida(renderer, font, &JugadorActual);
        }
        enviar_jugadores_con_curl("https://algoritmos-api.azurewebsites.net/api/TaCTi", "buffer", &JugadorActual, 1);
        elementosRestantes = contar_elementos_lista(p);
    }
    vaciar_lista(p);

    #ifdef WIN_32
        Sleep(1000);
    #else
        sleep(1);
    #endif

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);
    SDL_RenderPresent(renderer);
}

void jugarPartida(SDL_Renderer* renderer, TTF_Font* font, Jugador* jugador){
    Tablero tablero;
    inicializarTablero(&tablero);

    char fichaJugador;
    char fichaMaquina;

    int turnoJugador = 1; // Empieza el jugador
    int jugando = 1;

    srand((unsigned int)time(NULL));
    turnoJugador = rand() % 2;

    if(turnoJugador == 1)
    {
        fichaJugador = 'X';
        fichaMaquina = 'O';
    }
    else
    {
        fichaJugador = 'O';
        fichaMaquina = 'X';
    }

    SDL_Color colorTexto = {255, 255, 255, 255};
    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};

    while (jugando)
    {
        // Limpiar solo el área derecha
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        // Dibujar el tablero
        dibujarTablero(renderer, font, &tablero);
        renderizarJugador(renderer, font, jugador, colorTexto, 450, 10);

        SDL_RenderPresent(renderer);

        SDL_Event e;
        SDL_WaitEvent(&e);
        if (e.type == SDL_QUIT)
        {
            jugando = 0;
            break;
        }

        if (turnoJugador && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            int x = e.button.x;
            int y = e.button.y;

            if (clickEnTablero(&tablero, x, y, fichaJugador)) // jugador juega con 'X'
            {
                if (hayGanador(&tablero) == fichaJugador) {
                    jugador->puntaje += 3; // GANÓ, sumar puntaje
                    jugando = 0;
                }
                else if (tableroLleno(&tablero)) {
                    jugador->puntaje += 2;
                    jugando = 0; //EMPATE
                }
                else {
                    turnoJugador = 0; // Pasa turno a la máquina
                }
            }
        }
        else if (!turnoJugador)
        {
            // Turno de la máquina
            maquinaJuega(&tablero, fichaMaquina);

            if (hayGanador(&tablero) == fichaMaquina) {
                jugador->puntaje -= 1;
                jugando = 0; //PERDIÓ
            }
            if (tableroLleno(&tablero)) {
                jugador->puntaje += 2; // EMPATE
                jugando = 0;
            }
            turnoJugador = 1; // Vuelve a ser el turno del jugador
        }
        if(!jugando)
        {
            SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
            SDL_RenderFillRect(renderer, &areaDerecha);

            // Dibujar el tablero
            dibujarTablero(renderer, font, &tablero);
            renderizarJugador(renderer, font, jugador, colorTexto, 450, 10);

            SDL_RenderPresent(renderer);
        }
    }
}

void inicializarTablero(Tablero* t){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            t->matriz[i][j] = ' '; // Espacio vacío
        }
    }
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

void maquinaJuega(Tablero* t, char ficha){
    int vacias[9][2]; // Máximo 9 celdas vacías, guardamos (fila, columna)
    int cantVacias = 0;
    int fila;
    int col;
    int eleccion,filaElegida,colElegida;

    // Buscar todas las celdas vacías
    for (fila = 0; fila < 3; fila++)
    {
        for (col = 0; col < 3; col++)
        {
            if (t->matriz[fila][col] == ' ')
            {
                vacias[cantVacias][0] = fila;
                vacias[cantVacias][1] = col;
                cantVacias++;
            }
        }
    }

    // Si no hay más lugares vacíos, no hace nada
    if (cantVacias == 0)
        return;

    // Elegir una celda vacía al azar
    eleccion = rand() % cantVacias;
    filaElegida = vacias[eleccion][0];
    colElegida = vacias[eleccion][1];

    // Colocar 'O' en la celda elegida
    t->matriz[filaElegida][colElegida] = ficha;
}

char hayGanador(Tablero* t){
    // Revisar filas
    int i,j;

    for (i = 0; i < 3; i++)
    {
        if (t->matriz[i][0] != ' ' && t->matriz[i][0] == t->matriz[i][1] && t->matriz[i][1] == t->matriz[i][2])
            return t->matriz[i][0];
    }

    // Revisar columnas
    for (j = 0; j < 3; j++)
    {
        if (t->matriz[0][j] != ' ' && t->matriz[0][j] == t->matriz[1][j] && t->matriz[1][j] == t->matriz[2][j])
            return t->matriz[0][j];
    }

    // Revisar diagonales
    if (t->matriz[0][0] != ' ' && t->matriz[0][0] == t->matriz[1][1] && t->matriz[1][1] == t->matriz[2][2])
        return t->matriz[0][0];

    if (t->matriz[0][2] != ' ' && t->matriz[0][2] == t->matriz[1][1] && t->matriz[1][1] == t->matriz[2][0])
        return t->matriz[0][2];

    // No hay ganador
    return ' ';
}

int tableroLleno(Tablero* t){
    int i,j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (t->matriz[i][j] == ' ')
                return 0; // Hay al menos un espacio libre
        }
    }
    return 1; // Todo lleno
}

void imprimirJugador(const void* dato) {
    const Jugador* jugador = (const Jugador*)dato;
    printf("- %s (Puntaje: %d)\n", jugador->nombre, jugador->puntaje);
}

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

void pedirCantidadJugadores(SDL_Renderer* renderer, TTF_Font* font, int* cantidadJugadores) {

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};
    SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
    SDL_RenderFillRect(renderer, &areaDerecha);

    // Texto de pregunta
    SDL_Rect pregunta = {(areaDerecha.x + (areaDerecha.w) / 2), 100, 0, 0};
    renderizarTexto(renderer, "Cuantos jugadores?", font, colorTexto, &pregunta);

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
                    entrada[indice] = e.text.text[0];
                    indice++;
                    entrada[indice] = '\0';
                }
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                    indice--;
                    entrada[indice] = '\0';
                }
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    salirCaptura = 1;
                }
            }
        }

        // Redibujar cada vez que cambia
        SDL_SetRenderDrawColor(renderer, 33, 33, 33, 255);
        SDL_RenderFillRect(renderer, &areaDerecha);

        renderizarTexto(renderer, "Cuantos jugadores?", font, colorTexto, &pregunta);

        if(strlen(entrada) > 0) {
            SDL_Rect respuesta = {(areaDerecha.x + (areaDerecha.w) / 2), 200, 0, 0};
            renderizarTexto(renderer, entrada, font, colorTexto, &respuesta);
        }

        SDL_RenderPresent(renderer);
    }

    *cantidadJugadores = atoi(entrada); // Convertir entrada a número
}

void pedirNombres(SDL_Renderer* renderer, TTF_Font* font, int cantidadJugadores, t_lista* p) {
    fflush(stdin);

    SDL_Color colorTexto = {255, 255, 255, 255};

    SDL_Rect areaDerecha = {250, 0, VENTANA_ANCHO - 250, VENTANA_ALTO};

    int i;

    for (i = 0; i < cantidadJugadores; i++) {

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
                        entrada[indice] = e.text.text[0];
                        indice++;
                        entrada[indice] = '\0';
                    }
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_BACKSPACE && indice > 0) {
                        indice--;
                        entrada[indice] = '\0';
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
            int anchoTexto = 0;
            int altoTexto = 0;
            if(strlen(entrada) > 0) {
                TTF_SizeText(font,entrada,&anchoTexto,&altoTexto);
                SDL_Rect respuestaRect = {areaDerecha.x + (areaDerecha.w - anchoTexto) / 2, 200, anchoTexto, altoTexto};
                renderizarTexto(renderer, entrada, font, colorTexto, &respuestaRect);
            }

            // Presentar la actualización de la pantalla
            SDL_RenderPresent(renderer);
        }

        // Guardar el nombre en la lista
        Jugador ingreso;
        memset(&ingreso,0,sizeof(Jugador));
        strncpy(ingreso.nombre,entrada,(sizeof(ingreso.nombre)-1));
        ingreso.nombre[sizeof(ingreso.nombre)-1] = '\0';
        printf("\nNombre guardado: %s\n",ingreso.nombre);
        ingreso.puntaje = 0;

        poner_al_comienzo_lista(p, &ingreso, sizeof(Jugador));
    }
}

void cargarJugadoresPrueba(t_lista* pl) {
    const char* nombres[] = {"Jugador 1", "Jugador 2", "Jugador 3"};
    int i;
    Jugador jugador;

    for (i = 0; i < 3; i++) {
        strncpy(jugador.nombre,nombres[i],sizeof(jugador.nombre)-1);
        jugador.nombre[sizeof(jugador.nombre)-1] = '\0';
        jugador.puntaje = 0;
        poner_al_final_lista(pl, &jugador, sizeof(Jugador));
    }
}

void leerArchivo(char* url, char* key, int* cantPartidas){
    FILE* archivo = fopen("config.txt", "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return;
    }

    // Leer URL y key
    fscanf(archivo, " %99[^|]| %99[^\n]", url, key);

    // Leer número entero en la segunda línea
    if(fscanf(archivo, "%d", cantPartidas) != 1)
    {
        perror("Error al leer cantidad de partidas.\n");
        *cantPartidas = 0;
    }

    fclose(archivo);
}
