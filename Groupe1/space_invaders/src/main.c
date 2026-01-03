#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "view_console.h"
#include "view_sdl.h"
#include "view_menu.h"
#include "highscores.h"

/* Programme principal
 * - Parse les arguments de la ligne de commande pour choisir la vue (--view=console|sdl)
 * - Crée l'état du jeu
 * - Lance la boucle de la vue choisie
 * - Détruit l'état du jeu et retourne un code de sortie
 */
int main(int argc, char** argv) {
    /* Vue par défaut : "console" */
    const char* view = "console";

    /* Parcours des arguments pour détecter "--view=...".
     * On commence à 1 pour ignorer argv[0] (nom du programme).
     * strncmp permet de comparer les premiers 7 caractères.
     * Si correspondance, view pointe vers la sous-chaîne après "--view=".
     */
    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "--view=", 7) == 0) view = argv[i] + 7;
    }

    /* Charger les meilleurs scores */
    HighScoreList* highscores = highscores_charger();
    if (!highscores) {
        fprintf(stderr, "Échec du chargement des meilleurs scores\n");
        return 1;
    }

    int rc = 0; /* Code de retour */
    int continuer_jeu = 1;

    /* Boucle principale du menu */
    while (continuer_jeu) {
        int choix_menu;
        
        /* Afficher le menu principal */
        if (strcmp(view, "console") == 0) {
            choix_menu = vue_console_menu_principal();
        } else if (strcmp(view, "sdl") == 0) {
            choix_menu = vue_sdl_menu_principal();
        } else {
            fprintf(stderr, "Vue inconnue '%s'\n", view);
            rc = 2;
            break;
        }

        /* Traiter le choix du menu */
        if (choix_menu == MENU_JOUER) {
            /* Créer l'état du jeu */
            EtatJeu* e = etatjeu_creer(80, 24);
            if (!e) {
                fprintf(stderr, "Échec de création de l'état du jeu\n");
                rc = 1;
                break;
            }

            /* Lancer la partie */
            if (strcmp(view, "console") == 0) {
                rc = vue_console_executer(e);
            } else if (strcmp(view, "sdl") == 0) {
                rc = vue_sdl_executer(e);
            }

            /* Vérifier si c'est un nouveau meilleur score */
            int score_final = etatjeu_obtenir_score(e);
            if (highscores_est_top5(highscores, score_final)) {
                /* Demander le nom du joueur */
                char* nom_joueur;
                if (strcmp(view, "console") == 0) {
                    nom_joueur = vue_console_saisir_nom(score_final, highscores);
                } else {
                    nom_joueur = vue_sdl_saisir_nom(score_final, highscores);
                }

                /* Insérer le score */
                highscores_inserer(highscores, score_final, nom_joueur);
                highscores_sauvegarder(highscores);
                free(nom_joueur);
            }

            /* Libérer l'état du jeu */
            etatjeu_detruire(e);
        } else if (choix_menu == MENU_VOIR_HIGHSCORES || choix_menu == MENU_HIGHSCORES) {
            /* Afficher les meilleurs scores */
            if (strcmp(view, "console") == 0) {
                vue_console_afficher_highscores(highscores);
            } else if (strcmp(view, "sdl") == 0) {
                vue_sdl_afficher_highscores(highscores);
            }
        } else if (choix_menu == MENU_OPTIONS) {
            if (strcmp(view, "sdl") == 0) {
                vue_sdl_menu_options();
            } else if (strcmp(view, "console") == 0) {
                vue_console_menu_options();
            }
        } else if (choix_menu == MENU_QUITTER) {
            continuer_jeu = 0;
        }
    }

    /* Libérer les meilleurs scores */
    highscores_detruire(highscores);

    /* Retour du code d'exécution */
    return rc;
}
