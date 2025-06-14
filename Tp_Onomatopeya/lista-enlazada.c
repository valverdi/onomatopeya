#include "lista-enlazada.h"

void crear_lista(t_lista* pl)
{
    *pl = NULL;
}

int lista_vacia(const t_lista* pl)
{
    return *pl==NULL;
}

int lista_llena(const t_lista* pl, size_t tamElem)
{
    Nodo* nodo = (Nodo*) malloc(sizeof(Nodo));

    if(!nodo)
        return LISTA_LLENA;

    nodo->elem = malloc(tamElem);

    if(!nodo->elem)
    {
        free(nodo);
        return LISTA_LLENA;
    }

    return 0;
}

int poner_al_comienzo_lista(t_lista* pl, const void* dato, size_t tamElem)
{
    Nodo* nuevoNodo = (Nodo*) malloc(sizeof(Nodo));

    if(!nuevoNodo)
        return LISTA_LLENA;

    nuevoNodo->elem = malloc(tamElem);

    if(!nuevoNodo->elem)
    {
        free(nuevoNodo);
        return LISTA_LLENA;
    }

    memcpy(nuevoNodo->elem, dato, tamElem);
    nuevoNodo->tamElem = tamElem;
    nuevoNodo->nodoSig = *pl;

    *pl = nuevoNodo;

    return OK;
}

int poner_al_final_lista(t_lista* pl, const void* dato, size_t tamElem)
{
    Nodo* nuevoNodo = (Nodo*) malloc(sizeof(Nodo));

    if(!nuevoNodo)
        return LISTA_LLENA;

    nuevoNodo->elem = malloc(tamElem);

    if(!nuevoNodo->elem)
    {
        free(nuevoNodo);
        return LISTA_LLENA;
    }

    nuevoNodo->nodoSig = NULL;
    nuevoNodo->tamElem = tamElem;
    memcpy(nuevoNodo->elem, dato, tamElem);

    while(*pl)
    {
        pl = &(*pl)->nodoSig;
    }

    *pl = nuevoNodo;


    return OK;
}

int poner_ordenado_lista(t_lista* pl, const void* dato, size_t tamElem, int(*cmp)(const void*, const void*))
{
    Nodo* nuevoNodo;

    nuevoNodo = (Nodo*) malloc(sizeof(Nodo));

    if(!nuevoNodo)
        return LISTA_LLENA;

    nuevoNodo->elem = malloc(tamElem);

    if(!nuevoNodo->elem)
    {
        free(nuevoNodo);
        return LISTA_LLENA;
    }

    memcpy(nuevoNodo->elem, dato, tamElem);
    nuevoNodo->tamElem = tamElem;


    while(*pl  && cmp(dato, (*pl)->elem)>0)
    {
        pl = &(*pl)->nodoSig;
    }

    nuevoNodo->nodoSig = *pl;
    *pl = nuevoNodo;

    return OK;

}

void vaciar_lista(t_lista* pl)
{
    Nodo* nodoAelim;

    while(*pl)
    {
        nodoAelim = *pl;
        *pl = nodoAelim->nodoSig;

        free(nodoAelim->elem);
        free(nodoAelim);
    }


}

int sacar_primero_lista(t_lista* pl, void* dato, size_t tamElem)
{
    Nodo* nodoAelim;

    if(!*pl)
        return LISTA_VACIA;

    nodoAelim = *pl;

    *pl = nodoAelim->nodoSig;

    memcpy(dato, nodoAelim->elem,tamElem);

    free(nodoAelim->elem);
    free(nodoAelim);

    return 0;
}

int contar_elementos_lista(t_lista* pl) {
    int cont = 0;
    Nodo* aux = *pl;
    while (aux) {
        cont++;
        aux = aux->nodoSig;
    }
    return cont;
}

int sacar_de_pos_lista(t_lista* pl, void* dato, size_t tamElem, unsigned int pos)
{
    Nodo* nodoAelim;

    while(*pl && pos > 0)
    {
        pl =&(*pl)->nodoSig;
        pos--;
    }


    if(!*pl)
        return -1;


    nodoAelim = *pl;
    memcpy(dato, nodoAelim->elem,MENOR(tamElem,nodoAelim->tamElem));


    *pl = nodoAelim->nodoSig;

    free(nodoAelim->elem);
    free(nodoAelim);

    return OK;
}

int sacar_ultimo_lista(t_lista* pl, void* dato, size_t tamElem)
{
    Nodo* nodoAelim;

    if(!*pl)
        return LISTA_VACIA;

    while((*pl)->nodoSig)
    {
        pl=&(*pl)->nodoSig;
    }

    nodoAelim = *pl;
    *pl = nodoAelim->nodoSig;

    memcpy(dato, nodoAelim->elem,MENOR(nodoAelim->tamElem,tamElem));
    free(nodoAelim->elem);
    free(nodoAelim);

    return OK;

}

void mostrar_lista(t_lista* pl, void(*prnt)(const void*))
{
    while(*pl)
    {
        prnt((*pl)->elem);
        pl=&(*pl)->nodoSig;
    }

    printf("\n");


}

int ver_primero_lista(const t_lista* pl, void* dato, size_t tamElem)
{
    if(!*pl)
        return LISTA_VACIA;

    memcpy(dato, (*pl)->elem, MENOR((*pl)->tamElem,tamElem));

    return OK;

}

Nodo* buscar_nodo_menor(Nodo* ini, int(*cmp)(const void*, const void*))
{
    Nodo* menor  = ini;
    ini = ini->nodoSig;

    while(ini)
    {
        if(cmp(ini->elem, menor->elem)<0)
        {
            menor = ini;
        }

        ini = ini->nodoSig;
    }

    return menor;
}

Nodo* sacar_nodo_menor(t_lista* pl, int(*cmp)(const void*, const void*))
{
    Nodo* menor = *pl;
    Nodo** anteriorAlMenor  = pl;

    if(!*pl)
        return NULL;

    pl=&(*pl)->nodoSig;

    while(*pl)
    {

        if(cmp(menor->elem, (*pl)->elem)>0)
        {
            anteriorAlMenor = pl;
            menor = *pl;
        }

        pl = &(*pl)->nodoSig;

    }

    *anteriorAlMenor = menor->nodoSig;

    return menor;

}

int ordenar_lista_seleccion(t_lista* pl, int(*cmp)(const void*, const void*))
{
    Nodo* aux;

    if(!*pl)
        return LISTA_VACIA;


    while(*pl)
    {
       aux = sacar_nodo_menor(pl, cmp);
       aux->nodoSig = *pl;
       *pl = aux;
       pl=&(*pl)->nodoSig;
    }

    return OK;
}

Nodo* buscarXclave(t_lista* pl, void* clave, size_t tamClave, int(*cmp)(const void*, const void*))
{
    Nodo* nodoClave = NULL;

    while(*pl)
    {
        if(cmp(clave, (*pl)->elem)==0)
            nodoClave = *pl;

        pl=&(*pl)->nodoSig;
    }


    return nodoClave;
}

void map_lista(const t_lista* pl, void(*accion)(void*, void*), void* extra)
{
    while(*pl)
    {
        accion((*pl)->elem,extra);
        pl=&(*pl)->nodoSig;
    }
}