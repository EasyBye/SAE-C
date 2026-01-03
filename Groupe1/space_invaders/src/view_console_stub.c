/*
 * view_console_stub.c
 * Stub pour la vue console quand ncurses n'est pas disponible.
 * Utilisé lors de la compilation sans ncurses.
 */

#include "view_console.h"
#include <stdio.h>

int vue_console_executer(EtatJeu* e) {
    (void)e;
    fprintf(stderr, "Vue console non disponible (ncurses non installé).\n");
    fprintf(stderr, "Installation de ncurses :\n");
    fprintf(stderr, "  Linux : sudo apt install libncursesw5-dev\n");
    fprintf(stderr, "  MSYS2 : pacman -S mingw-w64-ucrt-x86_64-ncurses\n");
    return 1;
}
