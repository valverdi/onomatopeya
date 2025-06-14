# onomatopeya
Tp de Algoritmos

Librerias externas:

SDL2: Agregada ya al proyecto
SDL2_TTF: Agregada ya al proyecto



Para compilar y ejecutar el programa desde sistemas operativos que no sean Windows:


1. Instalar localmente SDL2 y SDL2_TTF, lo más recomendado es ejecutar el siguiente comando desde una terminal: 
    En Linux:
    sudo apt-get install libsdl2-dev libsdl2-ttf-dev
    
    En macOS:
    brew install SDL2 SDL2_TTF

2. Instalar localmente libcurl, nuevamente desde una terminal:
    En Linux:
    sudo apt-get install libcurl4-openssl-dev

    En macOS:
    brew install curl

3. Usando el comando cd en una terminal, moverse hasta la carpeta raiz del proyecto (debería ser Tp_Onomatopeya)

4. En una terminal, compilar utilizando:
    gcc -o Tp_Onomatopeya main.c api.c lista-enlazada.c cJson.c macros.c \
    -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_ttf

5. Si se compiló correctamente, desde la misma terminal ejecutar:
    ./Tp_Onomatopeya