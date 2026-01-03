# Architecture (MVC)

## Structure du projet

```
space_invaders/
├── include/
│   ├── model.h              # API du jeu (état, règles)
│   ├── controller.h         # Traduction commandes → actions
│   ├── view_console.h       # Interface ncurses
│   ├── view_sdl.h           # Interface SDL3
│   ├── view_menu.h          # Menus (console & SDL3)
│   ├── highscores.h         # Gestion des high-scores
│   └── text_bitmap.h        # Police bitmap pour SDL3
├── src/
│   ├── model.c              # Logique du jeu (pur, pas d'UI)
│   ├── controller.c         # Exécution des commandes
│   ├── main.c               # Boucle principale
│   ├── view_console.c       # Rendu ncurses
│   ├── view_console_stub.c  # Stub si ncurses manque
│   ├── view_menu_console.c  # Menu principal console
│   ├── view_sdl.c           # Rendu SDL3
│   ├── view_sdl_stub.c      # Stub si SDL3 manque
│   ├── view_menu_sdl.c      # Menu principal SDL3
│   ├── highscores.c         # Chargement/sauvegarde JSON
│   └── text_bitmap.c        # Bitmap font SDL3
├── data/
│   └── highscores.json      # Top 5 scores persistants
├── Makefile                 # Build avec détection auto
└── valgrind.supp            # Suppressions pour fuites libs
```

## Modèle
- `include/model.h` / `src/model.c`
- État du jeu (vaisseau, ennemis, tirs, score, vies, niveau) et règles (collisions, progression).
- 100% indépendant des bibliothèques d’affichage.

## Contrôleur
- `include/controller.h` / `src/controller.c`
- Reçoit des `Commande` (gauche, droite, tirer, pause, quitter) et appelle l’API du modèle.

## Vues
- Console : `src/view_console.c`
	- ncurses, rendu texte, throttle de rafraîchissement, menu Options pour reconfigurer les touches.
- SDL3 : `src/view_sdl.c`
	- Rendu 800×600, bitmap font, menu Options pour remapper les touches, pause en jeu.
- Menus : `src/view_menu_console.c`, `src/view_menu_sdl.c` gèrent les écrans titre/options/scores et la saisie de nom pour high-score.
- Stubs : `src/view_console_stub.c`, `src/view_sdl_stub.c` quand une dépendance manque.

## High-scores
- `src/highscores.c` lit/écrit `data/highscores.json` (top 5).
- Insertion après partie si le score est éligible, saisie du nom via la vue active.

## Keybindings
- Structures globales pour chaque vue (`ConsoleKeyBindings`, `KeyBindings`).
- Menus Options permettent de modifier les touches avec détection de conflits.

## Boucle principale
- `src/main.c` charge les scores, affiche le menu principal, démarre la vue choisie (`--view=console`/`--view=sdl`), puis sauvegarde les scores si besoin.

