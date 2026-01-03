#include "view_console.h"
#include "controller.h"

#include <ncursesw/curses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static ConsoleKeyBindings g_bindings = { 'a', 'd', ' ', 'p', 'q' };

void vue_console_get_bindings(ConsoleKeyBindings* out) {
    if (out) *out = g_bindings;
}

void vue_console_set_bindings(const ConsoleKeyBindings* in) {
    if (in) g_bindings = *in;
}

static int lire_touche_non_bloquant(void) {
    int c = getch();
    if (c == ERR) return -1;
    return c;
}

/* Rendu console basé sur ncursesw. Touches : 'a'/'LEFT' gauche, 'd'/'RIGHT' droite,
 * 'espace' tirer, 'p' pause, 'q' quitter
 */
int vue_console_executer(EtatJeu* e) {
    if (!e) return -1;

    /* initialisation ncurses */
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    refresh();

    /* couleurs : appeler start_color() puis activer si possible.
     * Certains environnements rapportent COLORS via terminfo even si has_colors() est false,
     * donc on vérifie aussi 'COLORS > 1' comme condition de secours.
     */
    int couleurs_actives = 0;
    start_color();
    if (has_colors() || COLORS > 1) {
        /* Utiliser un fond noir explicite pour garantir le contraste dans les terminaux
         * où le fond par défaut ne s'affiche pas correctement. Utiliser aussi
         * l'attribut gras lors du rendu pour améliorer la visibilité des couleurs.
         */
        init_pair(1, COLOR_RED, COLOR_BLACK);     /* ennemis */
        init_pair(2, COLOR_CYAN, COLOR_BLACK);    /* vaisseau */
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  /* projectile joueur */
        init_pair(4, COLOR_MAGENTA, COLOR_BLACK); /* projectile ennemi */
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   /* UI / texte */
        init_pair(6, COLOR_GREEN, COLOR_BLACK);   /* boucliers */
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);  /* particules explosion */
        couleurs_actives = 1;
    }

    int hauteur_term = 0, largeur_term = 0;
    getmaxyx(stdscr, hauteur_term, largeur_term);
    (void)largeur_term;

    const int largeur = etatjeu_obtenir_largeur(e);
    int hauteur = hauteur_term - 2;
    if (hauteur <= 0) hauteur = 24;

    const int ips = 20;
    const int temps_image_ms = 1000 / ips;
    int en_pause = 0;

    int prev_score = -1, prev_vies = -1, prev_niveau = -1;
    int prev_pause = -1;

    /* buffer d'écran */
    char* tampon = malloc((size_t)largeur * hauteur);
    char* tampon_prev = malloc((size_t)largeur * hauteur);
    if (!tampon || !tampon_prev) { free(tampon); free(tampon_prev); endwin(); return -1; }
    memset(tampon_prev, 0, (size_t)largeur * hauteur);

    while (!etatjeu_devrait_quitter(e)) {
        /* Si game over, afficher message et attendre choix */
        if (etatjeu_est_game_over(e)) {
            clear();
            if (couleurs_actives) attron(COLOR_PAIR(1) | A_BOLD);
            else attron(A_BOLD);
            mvprintw(hauteur / 2 - 2, largeur / 2 - 10, "VOUS ETES MORT !");
            mvprintw(hauteur / 2, largeur / 2 - 15, "Score final: %d", etatjeu_obtenir_score(e));
            mvprintw(hauteur / 2 + 2, largeur / 2 - 20, "Appuyez sur 'r' pour recommencer");
            mvprintw(hauteur / 2 + 3, largeur / 2 - 20, "Appuyez sur 'q' pour quitter");
            if (couleurs_actives) attroff(COLOR_PAIR(1) | A_BOLD);
            else attroff(A_BOLD);
            refresh();
            
            /* Attendre choix */
            nodelay(stdscr, FALSE); /* Bloquer pour attendre la touche */
            int choix = getch();
            nodelay(stdscr, TRUE); /* Revenir en mode non-bloquant */
            
            if (choix == 'r' || choix == 'R') {
                etatjeu_reinitialiser(e);
                en_pause = 0;
                continue;
            } else if (choix == 'q' || choix == 'Q') {
                controleur_appliquer_commande(e, CMD_QUITTER);
                break;
            }
        }
        
        int touche = lire_touche_non_bloquant();
        int input_recu = (touche != -1);
        if (input_recu) {
            if (touche == g_bindings.quitter || touche == toupper(g_bindings.quitter)) { controleur_appliquer_commande(e, CMD_QUITTER); break; }
            if (touche == g_bindings.pause || touche == toupper(g_bindings.pause)) { en_pause = !en_pause; }
            if (touche == g_bindings.gauche || touche == toupper(g_bindings.gauche) || touche == KEY_LEFT) controleur_appliquer_commande(e, CMD_GAUCHE);
            if (touche == g_bindings.droite || touche == toupper(g_bindings.droite) || touche == KEY_RIGHT) controleur_appliquer_commande(e, CMD_DROITE);
            if (touche == g_bindings.tirer || touche == toupper(g_bindings.tirer) || touche == KEY_ENTER || touche == '\n' || touche == '\r') controleur_appliquer_commande(e, CMD_TIRER);
        }

        if (!en_pause) etatjeu_mettre_a_jour(e, 1.0 / ips);

        /* effacer le buffer */
        memset(tampon, ' ', (size_t)largeur * hauteur);

        /* dessiner les ennemis */
        int nombre_ennemis = etatjeu_obtenir_nombre_ennemis(e);
        for (int idx = 0; idx < nombre_ennemis; ++idx) {
            if (!etatjeu_ennemi_vivant(e, idx)) continue;
            int ennemi_x = etatjeu_obtenir_ennemi_x(e, idx);
            int ennemi_y = etatjeu_obtenir_ennemi_y(e, idx);
            if (ennemi_y >= 0 && ennemi_y < hauteur && ennemi_x >= 0 && ennemi_x < largeur) tampon[ennemi_y * largeur + ennemi_x] = 'W';
        }

        /* dessiner les projectiles */
        int nombre_projectiles = etatjeu_obtenir_nombre_projectiles(e);
        for (int idx = 0; idx < nombre_projectiles; ++idx) {
            int proj_x = etatjeu_obtenir_projectile_x(e, idx);
            int proj_y = etatjeu_obtenir_projectile_y(e, idx);
            int proprietaire = etatjeu_obtenir_projectile_proprietaire(e, idx);
            if (proj_y >= 0 && proj_y < hauteur && proj_x >= 0 && proj_x < largeur) tampon[proj_y * largeur + proj_x] = proprietaire == 0 ? '|' : '!';
        }

        /* dessiner les boucliers */
        int nombre_boucliers = etatjeu_obtenir_nombre_boucliers(e);
        for (int idx = 0; idx < nombre_boucliers; ++idx) {
            if (!etatjeu_bouclier_vivant(e, idx)) continue;
            int bouclier_x = etatjeu_obtenir_bouclier_x(e, idx);
            int bouclier_y = etatjeu_obtenir_bouclier_y(e, idx);
            if (bouclier_y >= 0 && bouclier_y < hauteur && bouclier_x >= 0 && bouclier_x < largeur) tampon[bouclier_y * largeur + bouclier_x] = '#';
        }

        /* dessiner les particules d'explosion */
        int nombre_particules = etatjeu_obtenir_nombre_particules(e);
        for (int idx = 0; idx < nombre_particules; ++idx) {
            int part_x = etatjeu_obtenir_particule_x(e, idx);
            int part_y = etatjeu_obtenir_particule_y(e, idx);
            if (part_y >= 0 && part_y < hauteur && part_x >= 0 && part_x < largeur) tampon[part_y * largeur + part_x] = '*';
        }

        /* dessiner le vaisseau */
        int vaisseau_x = etatjeu_obtenir_vaisseau_x(e);
        int vaisseau_y = hauteur - 2;
        if (vaisseau_y >= 0 && vaisseau_y < hauteur && vaisseau_x >= 0 && vaisseau_x < largeur) tampon[vaisseau_y * largeur + vaisseau_x] = '^';

        int score_actuel = etatjeu_obtenir_score(e);
        int vies_actuelles = etatjeu_obtenir_vies(e);
        int niveau_actuel = etatjeu_obtenir_niveau(e);

        int contenu_change = memcmp(tampon, tampon_prev, (size_t)largeur * hauteur) != 0;
        int header_change = (score_actuel != prev_score) || (vies_actuelles != prev_vies) || (niveau_actuel != prev_niveau);
        int pause_change = (en_pause != prev_pause);

        if (input_recu || contenu_change || header_change || pause_change) {
            clear();
            if (couleurs_actives) {
                attron(COLOR_PAIR(5) | A_BOLD);
                mvprintw(0, 0, "Score: %d  Vies: %d  Level: %d", score_actuel, vies_actuelles, niveau_actuel);
                attroff(COLOR_PAIR(5) | A_BOLD);
            } else {
                attron(A_BOLD);
                mvprintw(0, 0, "Score: %d  Vies: %d  Level: %d", score_actuel, vies_actuelles, niveau_actuel);
                attroff(A_BOLD);
            }
            for (int lig = 0; lig < hauteur; ++lig) {
                for (int col = 0; col < largeur; ++col) {
                    int caractere = tampon[lig * largeur + col];
                    if (couleurs_actives) {
                        if (caractere == 'W') attron(COLOR_PAIR(1) | A_BOLD);
                        else if (caractere == '^') attron(COLOR_PAIR(2) | A_BOLD);
                        else if (caractere == '|') attron(COLOR_PAIR(3) | A_BOLD);
                        else if (caractere == '!') attron(COLOR_PAIR(4) | A_BOLD);
                        else if (caractere == '#') attron(COLOR_PAIR(6) | A_BOLD);
                        else if (caractere == '*') attron(COLOR_PAIR(7) | A_BOLD);
                        else attron(COLOR_PAIR(5) | A_BOLD);
                    }
                    mvaddch(lig + 1, col, caractere);
                    if (couleurs_actives) {
                        if (caractere == 'W') attroff(COLOR_PAIR(1) | A_BOLD);
                        else if (caractere == '^') attroff(COLOR_PAIR(2) | A_BOLD);
                        else if (caractere == '|') attroff(COLOR_PAIR(3) | A_BOLD);
                        else if (caractere == '!') attroff(COLOR_PAIR(4) | A_BOLD);
                        else if (caractere == '#') attroff(COLOR_PAIR(6) | A_BOLD);
                        else if (caractere == '*') attroff(COLOR_PAIR(7) | A_BOLD);
                        else if (caractere == '#') attroff(COLOR_PAIR(6) | A_BOLD);
                        else attroff(COLOR_PAIR(5) | A_BOLD);
                    }
                }
            }

            if (en_pause) {
                if (couleurs_actives) attron(COLOR_PAIR(5) | A_BOLD);
                else attron(A_BOLD);
                mvprintw(hauteur + 1, 0, "-- EN PAUSE --");
                if (couleurs_actives) attroff(COLOR_PAIR(5) | A_BOLD);
                else attroff(A_BOLD);
            }

            refresh();
            memcpy(tampon_prev, tampon, (size_t)largeur * hauteur);
            prev_score = score_actuel;
            prev_vies = vies_actuelles;
            prev_niveau = niveau_actuel;
            prev_pause = en_pause;
        }

        napms(temps_image_ms);
    }

    free(tampon);
    free(tampon_prev);
    endwin();
    return 0;
}
