#include "text_bitmap.h"

#include <ctype.h>

/* Rectangle utilitaire pour dessiner des glyphes */
static void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Color color) {
    SDL_FRect r = { (float)x, (float)y, (float)w, (float)h };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &r);
}

static int draw_glyph(SDL_Renderer* renderer, int x, int y, char ch, SDL_Color color, int pixel, int spacing) {
    char c = (char)toupper((unsigned char)ch);
    int advance = pixel * 4 + spacing; /* avancée par défaut */

    switch (c) {
        case 'A':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            break;
        case 'B':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel*2, y+pixel*3, pixel, pixel*2, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'C':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'D':
            draw_rect(renderer, x, y, pixel*2, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y+pixel, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*2, pixel, color);
            break;
        case 'E':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*2, pixel, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'F':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*2, pixel, color);
            break;
        case 'G':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel*2, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel*3, color);
            break;
        case 'H':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            break;
        case 'I':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'J':
            draw_rect(renderer, x, y+pixel*3, pixel*2, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'K':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*3, pixel, pixel*2, color);
            break;
        case 'L':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'M':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel, color);
            draw_rect(renderer, x+pixel*3, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel*4, y, pixel, pixel*5, color);
            advance = pixel * 5 + spacing;
            break;
        case 'N':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel, color);
            break;
        case 'O':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'P':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            break;
        case 'Q':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*4, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*4, color);
            draw_rect(renderer, x, y+pixel*3, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*4, pixel, pixel, color);
            break;
        case 'R':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*3, pixel, pixel*2, color);
            break;
        case 'S':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'T':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel, y, pixel, pixel*5, color);
            break;
        case 'U':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case 'V':
            draw_rect(renderer, x, y, pixel, pixel*3, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*3, color);
            draw_rect(renderer, x+pixel, y+pixel*3, pixel, pixel*2, color);
            break;
        case 'W':
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*3, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel, y+pixel*3, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel*2, y+pixel*3, pixel, pixel*2, color);
            advance = pixel * 5 + spacing;
            break;
        case 'X':
            draw_rect(renderer, x, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel, color);
            draw_rect(renderer, x, y+pixel*3, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel*2, y+pixel*3, pixel, pixel*2, color);
            break;
        case 'Y':
            draw_rect(renderer, x, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel*3, color);
            break;
        case 'Z':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel*2, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '0':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '1':
            draw_rect(renderer, x+pixel, y, pixel, pixel*5, color);
            advance = pixel * 3 + spacing;
            break;
        case '2':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*2, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel*2, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '3':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '4':
            draw_rect(renderer, x, y, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            break;
        case '5':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '6':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '7':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            break;
        case '8':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*5, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel*4, pixel*3, pixel, color);
            break;
        case '9':
            draw_rect(renderer, x, y, pixel*3, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel*3, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel*5, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            break;
        case ' ': /* espace */
            advance = pixel * 2 + spacing;
            break;
        case '>':
            draw_rect(renderer, x, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel, color);
            draw_rect(renderer, x+pixel*2, y, pixel, pixel, color);
            draw_rect(renderer, x+pixel*2, y+pixel*2, pixel, pixel, color);
            break;
        case '<':
            draw_rect(renderer, x+pixel*2, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y+pixel*2, pixel, pixel, color);
            draw_rect(renderer, x, y, pixel, pixel, color);
            draw_rect(renderer, x, y+pixel*2, pixel, pixel, color);
            break;
        case ':':
            draw_rect(renderer, x+pixel, y+pixel, pixel, pixel, color);
            draw_rect(renderer, x+pixel, y+pixel*3, pixel, pixel, color);
            advance = pixel * 3 + spacing;
            break;
        case '#':
            draw_rect(renderer, x, y+pixel, pixel, pixel*3, color);
            draw_rect(renderer, x+pixel*2, y+pixel, pixel, pixel*3, color);
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            draw_rect(renderer, x, y+pixel, pixel*3, pixel, color);
            break;
        case '-':
            draw_rect(renderer, x, y+pixel*2, pixel*3, pixel, color);
            break;
        case '.':
            draw_rect(renderer, x+pixel, y+pixel*4, pixel, pixel, color);
            advance = pixel * 2 + spacing;
            break;
        default:
            /* caractère non supporté : petit carré */
            draw_rect(renderer, x, y+pixel*4, pixel, pixel, color);
            advance = pixel * 2 + spacing;
            break;
    }

    return advance;
}

void bitmap_draw_text_custom(SDL_Renderer* renderer, int x, int y, const char* text, SDL_Color color, int pixel_size, int spacing) {
    if (!renderer || !text) return;
    int cursor_x = x;
    for (const char* ch = text; *ch; ++ch) {
        cursor_x += draw_glyph(renderer, cursor_x, y, *ch, color, pixel_size, spacing);
    }
}

void bitmap_draw_text(SDL_Renderer* renderer, int x, int y, const char* text, SDL_Color color) {
    bitmap_draw_text_custom(renderer, x, y, text, color, BITMAP_FONT_DEFAULT_SIZE, BITMAP_FONT_DEFAULT_SPACING);
}
