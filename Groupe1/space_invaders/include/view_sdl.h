/*
 * Interface pour une future vue SDL. Le fichier `src/view_sdl.c`
 * fourni dans ce dépôt est minimal (sans inclusion de SDL)
 * pour permettre la compilation sur des systèmes sans SDL3.
 *
 * Pour activer une vraie vue graphique : remplacer `src/view_sdl.c`
 * par une implémentation qui inclut <SDL3/SDL.h> et réalise le rendu.
 */
#ifndef VIEW_SDL_H
#define VIEW_SDL_H

#include "model.h"
#include <SDL3/SDL.h>

typedef struct {
	SDL_Keycode gauche;
	SDL_Keycode droite;
	SDL_Keycode tirer;
	SDL_Keycode pause;
	SDL_Keycode quitter;
} KeyBindings;

void vue_sdl_get_bindings(KeyBindings* out);
void vue_sdl_set_bindings(const KeyBindings* in);

/* Lance la vue SDL (stub par défaut). Retourne 0 si OK, sinon >0. */
int vue_sdl_executer(EtatJeu* e);

#endif /* VIEW_SDL_H */
