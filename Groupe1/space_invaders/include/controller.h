#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Le contrôleur traduit les commandes d'entrée en actions sur l'état du jeu */
#include "model.h"

/* Commandes possibles envoyées par les vues */
typedef enum Commande {
    CMD_GAUCHE,  /* déplacer vaisseau vers la gauche */
    CMD_DROITE,  /* déplacer vaisseau vers la droite */
    CMD_TIRER,   /* tirer un projectile */
    CMD_PAUSE,   /* pause (gérée par la vue) */
    CMD_QUITTER  /* quitter le jeu */
} Commande;

/* Applique une commande donnée sur l'état du jeu. */
void controleur_appliquer_commande(EtatJeu* e, Commande c);

#endif /* CONTROLLER_H */
