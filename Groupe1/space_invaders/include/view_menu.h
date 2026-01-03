#ifndef VIEW_MENU_H
#define VIEW_MENU_H

#include "model.h"
#include "highscores.h"

/* Mode de menu */
#define MENU_PRINCIPAL 0
#define MENU_HIGHSCORES 1
#define MENU_SAISIE_NOM 2

/* Retour des menus */
#define MENU_JOUER 1
#define MENU_VOIR_HIGHSCORES 2
#define MENU_QUITTER 3
#define MENU_RETOUR 4
#define MENU_OPTIONS 5

/* Affiche le menu principal et retourne le choix de l'utilisateur */
int vue_console_menu_principal(void);

/* Affiche les meilleurs scores */
void vue_console_afficher_highscores(HighScoreList* list);

/* Affiche le menu options (console) */
void vue_console_menu_options(void);

/* Demande le nom du joueur et retourne le nom saisi */
char* vue_console_saisir_nom(int score, HighScoreList* list);

/* Versions SDL */
int vue_sdl_menu_principal(void);
void vue_sdl_afficher_highscores(HighScoreList* list);
char* vue_sdl_saisir_nom(int score, HighScoreList* list);
void vue_sdl_menu_options(void);

#endif /* VIEW_MENU_H */
