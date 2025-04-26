#include "macros.h"

int cmpInt(const void* dato1, const void* dato2)
{
    return *((int*)dato1) - *((int*)dato2);
}

void prntInt(const void* dato)
{
    printf("- %d ", *((int*)dato));
}


void accionPrntInt(void* dato, void* extra)
{
    printf("- %d ", *((int*)dato));
}
