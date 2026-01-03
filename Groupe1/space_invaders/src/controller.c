#include "controller.h"
#include <stdio.h>

/*
 * Le contrôleur traduit des commandes d'entrée (`Commande`) en actions
 * sur le modèle (`EtatJeu`). Le contrôleur reste simple et délègue la
 * logique de jeu au modèle.
 */

/* Accès à une aide interne du module modèle pour définir le flag "quitter" */
extern void _etatjeu_definir_quitter(EtatJeu* e, int q);

/* Applique la commande `c` sur l'état `e`. Ne fait rien si `e` est NULL. */
void controleur_appliquer_commande(EtatJeu* e, Commande c) {
    if (!e) return;
    switch (c) {
        case CMD_GAUCHE:
            /* Déplace le vaisseau vers la gauche */
            etatjeu_deplacer_vaisseau(e, -1);
            break;
        case CMD_DROITE:
            /* Déplace le vaisseau vers la droite */
            etatjeu_deplacer_vaisseau(e, 1);
            break;
        case CMD_TIRER:
            /* Demande au modèle de tirer */
            etatjeu_vaisseau_tirer(e);
            break;
        case CMD_PAUSE:
            /* La pause est gérée par la vue ; le contrôleur n'a pas d'action */
            break;
        case CMD_QUITTER:
            /* Demande au modèle de signaler la fin du jeu */
            _etatjeu_definir_quitter(e, 1);
            break;
        default:
            break;
    }
}
