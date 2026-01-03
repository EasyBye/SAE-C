#ifndef TEXT_BITMAP_H
#define TEXT_BITMAP_H

#include <SDL3/SDL.h>

#define BITMAP_FONT_DEFAULT_SIZE 3
#define BITMAP_FONT_DEFAULT_SPACING 5

/* Dessine du texte en police bitmap (lettres A-Z, chiffres 0-9, ponctuation simple) */
void bitmap_draw_text(SDL_Renderer* renderer, int x, int y, const char* text, SDL_Color color);

/* Variante avec taille et espacement personnalisés */
void bitmap_draw_text_custom(SDL_Renderer* renderer, int x, int y, const char* text, SDL_Color color, int pixel_size, int spacing);

#endif /* TEXT_BITMAP_H */
