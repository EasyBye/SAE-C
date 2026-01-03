#ifndef HIGHSCORES_H
#define HIGHSCORES_H

typedef struct {
    int score;
    char nom[50];
} HighScore;

typedef struct {
    HighScore scores[5];
    int nombre_scores;
} HighScoreList;

/* Charge les meilleurs scores depuis le fichier JSON */
HighScoreList* highscores_charger(void);

/* Libère la mémoire des meilleurs scores */
void highscores_detruire(HighScoreList* list);

/* Vérifie si un score fait partie du top 5 */
int highscores_est_top5(HighScoreList* list, int score);

/* Insère un nouveau score (en supposant qu'il fait partie du top 5) */
void highscores_inserer(HighScoreList* list, int score, const char* nom);

/* Sauvegarde les meilleurs scores en JSON */
int highscores_sauvegarder(HighScoreList* list);

#endif /* HIGHSCORES_H */
