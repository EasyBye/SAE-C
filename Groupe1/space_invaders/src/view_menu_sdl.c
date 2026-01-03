/*
 * view_menu_sdl.c
 * Implémentation du menu principal en SDL3 (simple, sans police TTF)
 */

#include "view_menu.h"
#include "view_sdl.h"
#include "text_bitmap.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void keycode_label(SDL_Keycode code, char* buf, size_t sz) {
    const char* name = SDL_GetKeyName(code);
    if (!name) name = "?";
    snprintf(buf, sz, "%s", name);
}

static bool key_in_use(const KeyBindings* b, SDL_Keycode k, int ignore_index) {
    SDL_Keycode arr[5] = { b->gauche, b->droite, b->tirer, b->pause, b->quitter };
    for (int i = 0; i < 5; ++i) {
        if (i == ignore_index) continue;
        if (arr[i] == k) return true;
    }
    return false;
}

static int sdl_menu_simple(int mode_highscores, HighScoreList* list) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Space Invaders - Menu", 640, 480, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* r = SDL_CreateRenderer(win, NULL);
    int selection = 0;
    int running = 1;
    SDL_Color blanc = {255,255,255,255};
    SDL_Color jaune = {255,200,0,255};

    while (running) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_EVENT_QUIT) { running = 0; selection = MENU_QUITTER; }
            if (evt.type == SDL_EVENT_KEY_DOWN) {
                if (mode_highscores) { running = 0; selection = MENU_RETOUR; break; }
                if (evt.key.key == SDLK_UP) selection = (selection + 3) % 4;
                if (evt.key.key == SDLK_DOWN) selection = (selection + 1) % 4;
                if (evt.key.key == SDLK_RETURN || evt.key.key == SDLK_KP_ENTER) running = 0;
            }
        }

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255); SDL_RenderClear(r);
        bitmap_draw_text(r, 200, 80, "SPACE INVADERS", blanc);

        if (!mode_highscores) {
            bitmap_draw_text(r, 200, 170, selection==0?"> JOUER <":"  JOUER  ", selection==0?jaune:blanc);
            bitmap_draw_text(r, 200, 210, selection==1?"> SCORES <":"  SCORES  ", selection==1?jaune:blanc);
            bitmap_draw_text(r, 200, 250, selection==2?"> OPTIONS <":"  OPTIONS  ", selection==2?jaune:blanc);
            bitmap_draw_text(r, 200, 290, selection==3?"> QUITTER <":"  QUITTER  ", selection==3?jaune:blanc);
        } else {
            bitmap_draw_text(r, 200, 150, "MEILLEURS SCORES", blanc);
            if (list) {
                for (int i=0;i<list->nombre_scores && i<5;i++) {
                    char line[64];
                    snprintf(line, sizeof(line), "#%d %s %d", i+1, list->scores[i].nom, list->scores[i].score);
                    bitmap_draw_text(r, 180, 200 + i*40, line, jaune);
                }
            }
            bitmap_draw_text(r, 180, 200 + 5*40, "APPUYEZ SUR UNE TOUCHE", blanc);
        }
        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r); SDL_DestroyWindow(win);
    if (selection==1) return MENU_VOIR_HIGHSCORES;
    if (selection==2) return MENU_OPTIONS;
    if (selection==3) return MENU_QUITTER;
    return MENU_JOUER;
}

int vue_sdl_menu_principal(void) {
    return sdl_menu_simple(0, NULL);
}

void vue_sdl_afficher_highscores(HighScoreList* list) {
    (void)sdl_menu_simple(1, list);
}

void vue_sdl_menu_options(void) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Options", 640, 480, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* r = SDL_CreateRenderer(win, NULL);

    KeyBindings binds;
    vue_sdl_get_bindings(&binds);

    const char* actions[6] = {"GAUCHE", "DROITE", "TIRER", "PAUSE", "QUITTER", "RETOUR"};
    int selection = 0;
    int running = 1;
    SDL_Color blanc = {255,255,255,255};
    SDL_Color jaune = {255,200,0,255};
    char info[64] = "";

    while (running) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_EVENT_QUIT) { running = 0; break; }
            if (evt.type == SDL_EVENT_KEY_DOWN) {
                if (evt.key.key == SDLK_ESCAPE) { running = 0; break; }
                if (evt.key.key == SDLK_UP) selection = (selection + 5) % 6;
                if (evt.key.key == SDLK_DOWN) selection = (selection + 1) % 6;
                if (evt.key.key == SDLK_RETURN || evt.key.key == SDLK_KP_ENTER) {
                    if (selection == 5) { running = 0; break; }
                    /* Demander une nouvelle touche */
                    SDL_SetRenderDrawColor(r, 0,0,0,255); SDL_RenderClear(r);
                    bitmap_draw_text(r, 120, 200, "APPUYEZ SUR UNE TOUCHE", blanc);
                    SDL_RenderPresent(r);
                    SDL_Event wait_evt;
                    while (SDL_WaitEvent(&wait_evt)) {
                        if (wait_evt.type == SDL_EVENT_KEY_DOWN) {
                            SDL_Keycode k = wait_evt.key.key;
                            if (key_in_use(&binds, k, selection)) {
                                snprintf(info, sizeof(info), "Conflit: deja %s", actions[selection]);
                            } else {
                                info[0] = '\0';
                                switch (selection) {
                                    case 0: binds.gauche = k; break;
                                    case 1: binds.droite = k; break;
                                    case 2: binds.tirer = k; break;
                                    case 3: binds.pause = k; break;
                                    case 4: binds.quitter = k; break;
                                    default: break;
                                }
                            }
                            break;
                        }
                        if (wait_evt.type == SDL_EVENT_QUIT) { running = 0; break; }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255); SDL_RenderClear(r);
        bitmap_draw_text(r, 240, 60, "OPTIONS", blanc);

        char label[64];
        for (int i = 0; i < 6; ++i) {
            SDL_Color col = (i == selection) ? jaune : blanc;
            if (i < 5) {
                char keybuf[32];
                keycode_label((i==0)?binds.gauche:(i==1)?binds.droite:(i==2)?binds.tirer:(i==3)?binds.pause:binds.quitter, keybuf, sizeof(keybuf));
                snprintf(label, sizeof(label), "%s : %s", actions[i], keybuf);
            } else {
                snprintf(label, sizeof(label), "%s", actions[i]);
            }
            bitmap_draw_text(r, 120, 140 + i*40, label, col);
        }

        if (info[0]) {
            bitmap_draw_text(r, 120, 400, info, jaune);
        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }

    vue_sdl_set_bindings(&binds);
    SDL_DestroyRenderer(r); SDL_DestroyWindow(win);
}

char* vue_sdl_saisir_nom(int score, HighScoreList* list) {
    (void)list;
    if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Nouveau score", 640, 200, 0);
    SDL_Renderer* r = SDL_CreateRenderer(win, NULL);
    SDL_StartTextInput(win);
    char* nom = (char*)malloc(21);
    memset(nom, 0, 21);
    SDL_Color blanc = {255,255,255,255};
    SDL_Color jaune = {255,200,0,255};
    int running = 1;
    while (running) {
        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_EVENT_QUIT) { running = 0; break; }
            if (evt.type == SDL_EVENT_KEY_DOWN) {
                if (evt.key.key == SDLK_RETURN || evt.key.key == SDLK_KP_ENTER) { running = 0; break; }
                if (evt.key.key == SDLK_BACKSPACE && strlen(nom)>0) nom[strlen(nom)-1]='\0';
            }
            if (evt.type == SDL_EVENT_TEXT_INPUT) {
                if (strlen(nom) < 20) strncat(nom, evt.text.text, 1);
            }
        }
        SDL_SetRenderDrawColor(r,0,0,0,255); SDL_RenderClear(r);
        char ligne[64]; snprintf(ligne, sizeof(ligne), "Score: %d", score);
        bitmap_draw_text(r, 40, 40, "NOUVEAU MEILLEUR SCORE", jaune);
        bitmap_draw_text(r, 40, 80, ligne, blanc);
        bitmap_draw_text(r, 40, 120, "NOM:", blanc);
        bitmap_draw_text(r, 120, 120, nom[0]?nom:"_", jaune);
        SDL_RenderPresent(r); SDL_Delay(16);
    }
    SDL_StopTextInput(win);
    SDL_DestroyRenderer(r); SDL_DestroyWindow(win);
    if (strlen(nom)==0) strcpy(nom, "ANONYME");
    return nom;
}
