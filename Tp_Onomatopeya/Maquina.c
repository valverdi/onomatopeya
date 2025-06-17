#include "Maquina.h"

void maquinaJuega(Tablero* t, char fichaMaquina, char fichaJugador){
    int fila,col;

    if(!hayMovimientos(t->matriz))
        return;

    mejorJugada(t->matriz,&fila,&col,fichaJugador,fichaMaquina);

    if(fila != -1 && col != -1)
        t->matriz[fila][col] = fichaMaquina;
}

int evaluar(char tablero[3][3],char fichaJugador, char fichaMaquina){
    // Verificamos filas y columnas
    for (int i = 0; i < 3; i++)
    {
        if (tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2])
        {
            if (tablero[i][0] == fichaJugador)
                return +10;
            if (tablero[i][0] == fichaMaquina)
                return -10;
        }
        if (tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i])
        {
            if (tablero[0][i] == fichaJugador)
                return +10;
            if (tablero[0][i] == fichaMaquina)
                return -10;
        }
    }
    // Verificamos diagonales
    if ((tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]) || (tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]))
    {
        if (tablero[1][1] == fichaJugador)
            return +10;
        if (tablero[1][1] == fichaMaquina)
            return -10;
    }
    return 0; // Empate o juego en curso
}

int hayMovimientos(char tablero[3][3]){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (tablero[i][j] == VACIO)
                return 1; // hay movimientos
        }
    }
    return 0; // no hay movimientos
}

int minimax(char tablero[3][3],int esMaximizador,char fichaJugador,char fichaMaquina){
    int puntaje = evaluar(tablero,fichaJugador,fichaMaquina);
    int I,J;
    int mejor,actual;

    if(puntaje == -10 || puntaje == 10 || !hayMovimientos(tablero))
        return puntaje;

    if(esMaximizador)
    {
        mejor = -1000;
        for(I = 0;I < 3;I++)
        {
            for(J = 0; J < 3; J++)
            {
                if(tablero[I][J] == VACIO)
                {
                    tablero[I][J] = fichaJugador;
                    actual = minimax(tablero,0,fichaJugador,fichaMaquina);
                    mejor = (mejor > actual) ? mejor : actual;
                    tablero[I][J] = VACIO;
                }
            }
        }
        return mejor;
    }
    else
    {
        mejor = 1000;
        for(I = 0; I < 3; I++)
        {
            for(J = 0; J < 3; J++)
            {
                if(tablero[I][J] == VACIO)
                {
                    tablero[I][J] = fichaMaquina;
                    actual = minimax(tablero,1,fichaJugador,fichaMaquina);
                    mejor = (mejor < actual) ? mejor : actual;
                    tablero[I][J] = VACIO;
                }
            }
        }
        return mejor;
    }
}

void mejorJugada(char tablero[3][3],int *mejorFila,int *mejorCol,char fichaJugador,char fichaMaquina){
    int mejorValor = 1000; //maquina minimiza
    int I,J;
    int valor;
    *mejorFila = -1;
    *mejorCol = -1;

    for(I = 0; I < 3; I++)
    {
        for(J = 0; J < 3; J++)
        {
            if(tablero[I][J] == VACIO)
            {
                tablero[I][J] = fichaMaquina;
                valor = minimax(tablero,1,fichaJugador,fichaMaquina);
                tablero[I][J] = VACIO;
                if(valor < mejorValor)
                {
                    mejorValor = valor;
                    *mejorFila = I;
                    *mejorCol = J;
                }
            }
        }
    }
}
