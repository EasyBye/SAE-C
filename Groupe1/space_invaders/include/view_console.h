/*
 * Vue console simple (TTY) — sans dépendance `ncurses` (car ne fonctionne pas).
 *
 * Cette vue lit des touches (non-bloquantes) et rend l'état du jeu
 * sous forme ASCII. Contrôles : `a` gauche, `d` droite, `espace` tirer,
 * `p` pause, `q` quitter.
 */
#ifndef VIEW_CONSOLE_H
#define VIEW_CONSOLE_H

#include "model.h"
#include <ncursesw/curses.h>

typedef struct {
	int gauche;
	int droite;
	int tirer;
	int pause;
	int quitter;
} ConsoleKeyBindings;

void vue_console_get_bindings(ConsoleKeyBindings* out);
void vue_console_set_bindings(const ConsoleKeyBindings* in);

/* Démarre la boucle de la vue console.
 * @param e : pointeur vers l'état du jeu (modèle)
 * @return 0 si succès, >0 en cas d'erreur.
 */
int vue_console_executer(EtatJeu* e);

#endif /* VIEW_CONSOLE_H */
