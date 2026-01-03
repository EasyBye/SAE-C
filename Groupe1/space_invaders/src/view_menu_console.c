/*
 * view_menu_console.c
 * Implémentation du menu principal en ncurses
 */

#include "view_menu.h"
#include "view_console.h"
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static void key_label(int key, char* buf, size_t sz) {
    switch (key) {
        case KEY_LEFT: snprintf(buf, sz, "LEFT"); break;
        case KEY_RIGHT: snprintf(buf, sz, "RIGHT"); break;
        case KEY_UP: snprintf(buf, sz, "UP"); break;
        case KEY_DOWN: snprintf(buf, sz, "DOWN"); break;
        case ' ': snprintf(buf, sz, "SPACE"); break;
        case '\n': snprintf(buf, sz, "ENTER"); break;
        case '\r': snprintf(buf, sz, "ENTER"); break;
        default:
            if (isprint((unsigned char)key)) snprintf(buf, sz, "%c", key);
            else snprintf(buf, sz, "0x%X", key);
            break;
    }
}

int vue_console_menu_principal(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
    
    start_color();
    if (has_colors() || COLORS > 1) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    }
    
    int selection = 0;
    int quitter_menu = 0;
    int dirty = 1;
    
    while (!quitter_menu) {
        int ch = getch();
        if (ch == KEY_UP) { selection = (selection - 1 + 4) % 4; dirty = 1; }
        else if (ch == KEY_DOWN) { selection = (selection + 1) % 4; dirty = 1; }
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) { quitter_menu = 1; }

        if (!dirty) continue;

        clear();
        int hauteur, largeur;
        getmaxyx(stdscr, hauteur, largeur);

        /* Afficher le titre */
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, largeur/2 - 10, "SPACE INVADERS");
        attroff(COLOR_PAIR(1) | A_BOLD);

        /* Afficher les options */
        attron(COLOR_PAIR(2));
        mvprintw(5, largeur/2 - 5, "Menu Principal");
        attroff(COLOR_PAIR(2));

        /* Option 1 : Jouer */
        if (selection == 0) {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(8, largeur/2 - 3, "> JOUER <");
            attroff(COLOR_PAIR(3) | A_BOLD);
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(8, largeur/2 - 3, "  JOUER  ");
            attroff(COLOR_PAIR(2));
        }

        /* Option 2 : Meilleurs scores */
        if (selection == 1) {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(10, largeur/2 - 8, "> MEILLEURS SCORES <");
            attroff(COLOR_PAIR(3) | A_BOLD);
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(10, largeur/2 - 8, "  MEILLEURS SCORES  ");
            attroff(COLOR_PAIR(2));
        }

        /* Option 3 : Options */
        if (selection == 2) {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(12, largeur/2 - 6, "> OPTIONS <");
            attroff(COLOR_PAIR(3) | A_BOLD);
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(12, largeur/2 - 6, "  OPTIONS  ");
            attroff(COLOR_PAIR(2));
        }

        /* Option 4 : Quitter */
        if (selection == 3) {
            attron(COLOR_PAIR(3) | A_BOLD);
            mvprintw(14, largeur/2 - 4, "> QUITTER <");
            attroff(COLOR_PAIR(3) | A_BOLD);
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(14, largeur/2 - 4, "  QUITTER  ");
            attroff(COLOR_PAIR(2));
        }

        attron(COLOR_PAIR(2));
        mvprintw(hauteur - 2, 0, "Utilisez les fleches pour naviguer, ENTREE pour selectionner");
        attroff(COLOR_PAIR(2));

        refresh();
        dirty = 0;
    }
    
    endwin();
    
    switch (selection) {
        case 0: return MENU_JOUER;
        case 1: return MENU_VOIR_HIGHSCORES;
        case 2: return MENU_OPTIONS;
        case 3: return MENU_QUITTER;
        default: return MENU_QUITTER;
    }
}

void vue_console_menu_options(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    if (has_colors() || COLORS > 1) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    }

    const char* actions[6] = {"GAUCHE", "DROITE", "TIRER", "PAUSE", "QUITTER", "RETOUR"};
    ConsoleKeyBindings binds; vue_console_get_bindings(&binds);
    int selection = 0;
    int quitter = 0;
    int dirty = 1;
    char info[64] = "";

    while (!quitter) {
        int ch = getch();
        if (ch == KEY_UP) { selection = (selection + 5) % 6; dirty = 1; }
        else if (ch == KEY_DOWN) { selection = (selection + 1) % 6; dirty = 1; }
        else if (ch == '\n' || ch == KEY_ENTER) {
            if (selection == 5) { quitter = 1; dirty = 1; }
            else {
                /* prompt for new key */
                nodelay(stdscr, FALSE);
                echo();
                clear();
                mvprintw(5, 5, "Appuyez sur une touche pour %s", actions[selection]);
                refresh();
                int nk = getch();
                noecho();
                nodelay(stdscr, TRUE);
                int arr[5] = { binds.gauche, binds.droite, binds.tirer, binds.pause, binds.quitter };
                int conflict = 0;
                for (int i=0;i<5;i++) if (i!=selection && arr[i]==nk) conflict=1;
                if (conflict) {
                    snprintf(info, sizeof(info), "Conflit: deja utilise");
                } else {
                    info[0]='\0';
                    switch (selection) {
                        case 0: binds.gauche = nk; break;
                        case 1: binds.droite = nk; break;
                        case 2: binds.tirer = nk; break;
                        case 3: binds.pause = nk; break;
                        case 4: binds.quitter = nk; break;
                        default: break;
                    }
                }
                dirty = 1;
            }
        }

        if (!dirty) { napms(10); continue; }

        clear();
        int h,l; getmaxyx(stdscr,h,l);
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, l/2 - 5, "OPTIONS");
        attroff(COLOR_PAIR(1) | A_BOLD);

        for (int i=0;i<6;i++) {
            char keybuf[32];
            if (i<5) key_label((i==0)?binds.gauche:(i==1)?binds.droite:(i==2)?binds.tirer:(i==3)?binds.pause:binds.quitter, keybuf, sizeof(keybuf));
            else keybuf[0] = '\0';
            if (selection==i) attron(COLOR_PAIR(3) | A_BOLD); else attron(COLOR_PAIR(2));
            mvprintw(6+i*2, l/2 - 12, "%s : %s", actions[i], keybuf);
            if (selection==i) attroff(COLOR_PAIR(3) | A_BOLD); else attroff(COLOR_PAIR(2));
        }

        if (info[0]) {
            attron(COLOR_PAIR(3));
            mvprintw(h-2, 2, "%s", info);
            attroff(COLOR_PAIR(3));
        } else {
            attron(COLOR_PAIR(2));
            mvprintw(h-2, 2, "ENTER pour modifier, RETOUR pour quitter");
            attroff(COLOR_PAIR(2));
        }

        refresh();
        dirty = 0;
    }

    vue_console_set_bindings(&binds);
    endwin();
}

void vue_console_afficher_highscores(HighScoreList* list) {
    if (!list) return;
    
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    start_color();
    if (has_colors() || COLORS > 1) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    }
    
    clear();
    
    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);
    
    /* Titre */
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, largeur/2 - 8, "MEILLEURS SCORES");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    /* Afficher les scores */
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(4, largeur/2 - 15, "Rang  Nom                  Score");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    for (int i = 0; i < list->nombre_scores; ++i) {
        mvprintw(5 + i, largeur/2 - 15, " #%d   %-20s %5d", i + 1, list->scores[i].nom, list->scores[i].score);
    }
    
    attron(COLOR_PAIR(2));
    mvprintw(hauteur - 2, 0, "Appuyez sur une touche pour revenir au menu");
    attroff(COLOR_PAIR(2));
    
    refresh();
    getch();
    endwin();
}

char* vue_console_saisir_nom(int score, HighScoreList* list) {
    (void)list;  /* paramètre non utilisé */
    initscr();
    cbreak();
    echo();
    curs_set(1);
    keypad(stdscr, TRUE);
    
    start_color();
    if (has_colors() || COLORS > 1) {
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    }
    
    clear();
    
    int hauteur, largeur;
    getmaxyx(stdscr, hauteur, largeur);
    
    /* Afficher le message */
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(hauteur/2 - 3, largeur/2 - 15, "NOUVEAU MEILLEUR SCORE !");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    attron(COLOR_PAIR(2));
    mvprintw(hauteur/2 - 1, largeur/2 - 10, "Score: %d", score);
    attroff(COLOR_PAIR(2));
    
    attron(COLOR_PAIR(2));
    mvprintw(hauteur/2 + 1, largeur/2 - 15, "Entrez votre nom (max 20 caracteres):");
    attroff(COLOR_PAIR(2));
    
    /* Saisie du nom */
    char* nom = (char*)malloc(21);
    memset(nom, 0, 21);
    mvgetnstr(hauteur/2 + 2, largeur/2 - 15, nom, 20);
    
    /* Si le nom est vide, utiliser "ANONYME" */
    if (strlen(nom) == 0) {
        strcpy(nom, "ANONYME");
    }
    
    endwin();
    return nom;
}
