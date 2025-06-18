# onomatopeya
Tp de Algoritmos

Librerias/recursos externos:

SDL2: Agregada ya al proyecto.
SDL2_TTF: Agregada ya al proyecto.
cJson: Agregado al proyecto.
Libcurl: Debe instalarse manualmente.

Para configurar LibCurl (en Windows utilizando CodeBlocks):

1. Instalar Curl en Windows (https://curl.se/Windows/).
2. Descomprimir el .zip y pasar esa carpeta a una ubicación que no se vaya a borrar.
3. Agregar dentro de PATH (como variable de entorno) la ubicación a la carpeta 'bin'.
4. En CodeBlocks: Settings > Compiler > Linker Settings: Agregar ubicación a "lib\libcurl.dll.a".
5. En CodeBlocks: Settings > Compiler > Search directories > Compiler and Resource compiler: Agregar ubicación a "include".
6. En CodeBlocks: Settings > Compiler > Search directories > Linker: Agregar ubicación a "lib".


Para compilar y ejecutar el programa desde sistemas operativos que no sean Windows:

1. Instalar localmente SDL2 y SDL2_TTF, lo más recomendado es ejecutar el siguiente comando desde una terminal:
   
    En Linux:
    sudo apt-get install libsdl2-dev libsdl2-ttf-dev
    
    En macOS:
    brew install SDL2 SDL2_TTF

3. Instalar localmente libcurl, nuevamente desde una terminal:
   
    En Linux:
    sudo apt-get install libcurl4-openssl-dev

    En macOS:
    brew install curl

5. Usando el comando cd en una terminal, moverse hasta la carpeta raiz del proyecto (debería ser Tp_Onomatopeya)

6. En una terminal, compilar utilizando:
   
    gcc -o Tp_Onomatopeya main.c api.c lista-enlazada.c cJson.c macros.c \
    -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_ttf

8. Si se compiló correctamente, desde la misma terminal ejecutar:
   
    ./Tp_Onomatopeya

9. Debería ejecutarse el programa correctamente.


CONFIGURACIÓN DEL JUEGO:        

Si se desea cambiar el grupo del juego o la cantidad de partidas jugadas por cada jugador:

1. Abrir el archivo "config.txt" dentro de la carpeta del juego.
2. Debería visualizar un texto con el siguiente estilo:
    <url>|<código>
    <cantidad de partidas>
3. Si desea utilizar otro grupo, deberá cambiar <código> por el código correspondiente al grupo que desea utilizar (No recomendado).
4. Si desea cambiar la cantidad de partidas que jugará cada jugador, cambiar <cantidad de partidas> por el número deseado (Evitar números altos para no entorpecer la experiencia. Cualquier número <=0 terminará el juego automáticamente).
