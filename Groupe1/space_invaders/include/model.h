#ifndef MODEL_H
#define MODEL_H

typedef struct EtatJeu EtatJeu;

/* Crée et initialise l'état du jeu.
 * @param largeur : largeur du terrain de jeu en colonnes.
 * @param hauteur : hauteur du terrain de jeu en lignes.
 * @return pointeur vers un `EtatJeu` initialisé ou NULL en cas d'erreur.
 */
EtatJeu* etatjeu_creer(int largeur, int hauteur);

/* Libère les ressources associées à l'état du jeu. */
void etatjeu_detruire(EtatJeu* e);

/* Met à jour l'état du jeu.
 * @param dt : temps écoulé (en secondes) depuis la dernière mise à jour.
 */
void etatjeu_mettre_a_jour(EtatJeu* e, double dt);

/* Actions du vaisseau */
/* Déplace le vaisseau : dir = -1 gauche, 1 droite */
void etatjeu_deplacer_vaisseau(EtatJeu* e, int dir);
/* Le vaisseau tire un projectile vers le haut. */
void etatjeu_vaisseau_tirer(EtatJeu* e);

/* Accesseurs simples */
int etatjeu_obtenir_vaisseau_x(const EtatJeu* e);
int etatjeu_obtenir_largeur(const EtatJeu* e);
int etatjeu_obtenir_vies(const EtatJeu* e);
int etatjeu_obtenir_score(const EtatJeu* e);
int etatjeu_obtenir_niveau(const EtatJeu* e);
int etatjeu_devrait_quitter(const EtatJeu* e);
int etatjeu_est_game_over(const EtatJeu* e);
void etatjeu_reinitialiser(EtatJeu* e);

/* Constantes limites */
#define NB_MAX_ENNEMIS 64
#define NB_MAX_PROJECTILES 128
#define NB_MAX_BOUCLIERS 4
#define NB_MAX_PARTICULES 256

/* Constantes pour les types d'entités */
#define TYPE_JOUEUR 0
#define TYPE_ENNEMI_FAIBLE 1
#define TYPE_ENNEMI_FORT 2
#define TYPE_BOUCLIER 3

/* API ennemis / projectiles (getters en lecture seule pour les vues) */
int etatjeu_obtenir_nombre_ennemis(const EtatJeu* e);
int etatjeu_obtenir_ennemi_x(const EtatJeu* e, int idx);
int etatjeu_obtenir_ennemi_y(const EtatJeu* e, int idx);
int etatjeu_ennemi_vivant(const EtatJeu* e, int idx);
int etatjeu_obtenir_ennemi_sante(const EtatJeu* e, int idx);

int etatjeu_obtenir_nombre_projectiles(const EtatJeu* e);
int etatjeu_obtenir_projectile_x(const EtatJeu* e, int idx);
int etatjeu_obtenir_projectile_y(const EtatJeu* e, int idx);
int etatjeu_obtenir_projectile_proprietaire(const EtatJeu* e, int idx); /* 0=joueur,1=ennemi */

/* API boucliers */
int etatjeu_obtenir_nombre_boucliers(const EtatJeu* e);
int etatjeu_obtenir_bouclier_x(const EtatJeu* e, int idx);
int etatjeu_obtenir_bouclier_y(const EtatJeu* e, int idx);
int etatjeu_bouclier_vivant(const EtatJeu* e, int idx);

/* API particules (explosions) */
int etatjeu_obtenir_nombre_particules(const EtatJeu* e);
int etatjeu_obtenir_particule_x(const EtatJeu* e, int idx);
int etatjeu_obtenir_particule_y(const EtatJeu* e, int idx);
int etatjeu_obtenir_particule_type(const EtatJeu* e, int idx);
int etatjeu_obtenir_particule_ttl(const EtatJeu* e, int idx);

#endif /* MODEL_H */
