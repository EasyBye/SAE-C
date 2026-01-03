# Installation - Space Invaders

Prépare les dépendances pour compiler et exécuter le jeu. Pour le fonctionnement du Makefile, voir BUILD.md.

## Prérequis généraux
- Compilateur C (gcc/clang), GNU Make, pkg-config.
- ncurses (vue console) et SDL3 (vue graphique) en option selon la plateforme.

## Linux (Debian/Ubuntu)
```bash
sudo apt update
sudo apt install build-essential pkg-config libncursesw5-dev libsdl3-dev valgrind
```

## macOS (Homebrew)
```bash
brew install pkg-config ncurses sdl3
```

## Windows

### MSYS2 UCRT64 
```bash
pacman -Syu
pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-pkg-config \
  mingw-w64-ucrt-x86_64-ncurses \
  mingw-w64-ucrt-x86_64-SDL3
```

### MSYS2 MINGW64 
```bash
pacman -S --needed \
  mingw-w64-x86_64-toolchain \
  mingw-w64-x86_64-pkg-config \
  mingw-w64-x86_64-ncurses \
  mingw-w64-x86_64-SDL3
```

### WSL2 (Ubuntu)
Suivre les commandes Linux ci-dessus.

## Vérifier la détection
```bash
make check-deps
```
Affiche les vues disponibles et les flags récupérés via pkg-config.

## Chemins personnalisés
Si vos libs sont dans un préfixe non standard :
```bash
make CPPFLAGS="-I/chemin/include" LDFLAGS="-L/chemin/lib"
```

## Dépannage rapide
- `pkg-config: command not found` → installez pkg-config.
- `ncursesw/curses.h` ou `libSDL3` introuvables → installez les paquets ci-dessus et relancez `make check-deps`.
- Liens SDL3 qui échouent sous MSYS2 → vérifiez que vous utilisez la bonne invite (UCRT64 vs MINGW64).
