/*
 * view_sdl_stub.c
 * Stub pour la vue SDL quand SDL3 n'est pas disponible.
 * Utilisé lors de la compilation sans SDL3.
 */

#include "view_sdl.h"
#include <stdio.h>

int vue_sdl_executer(EtatJeu* e) {
    (void)e;
    fprintf(stderr, "Vue SDL3 non disponible (SDL3 non installé).\n");
    fprintf(stderr, "Installation de SDL3 :\n");
    fprintf(stderr, "  Linux : sudo apt install libsdl3-dev\n");
    fprintf(stderr, "  MSYS2 : pacman -S mingw-w64-ucrt-x86_64-SDL3\n");
    fprintf(stderr, "  macOS : brew install sdl3\n");
    return 1;
}
