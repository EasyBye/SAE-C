/*
 * highscores.c
 * Gestion des meilleurs scores avec sauvegarde/chargement JSON
 */

#include "highscores.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HIGHSCORES_FILE "data/highscores.json"

HighScoreList* highscores_charger(void) {
    HighScoreList* list = (HighScoreList*)malloc(sizeof(HighScoreList));
    if (!list) return NULL;
    
    list->nombre_scores = 0;
    memset(list->scores, 0, sizeof(list->scores));
    
    FILE* f = fopen(HIGHSCORES_FILE, "r");
    if (!f) {
        /* Fichier n'existe pas encore, retourner une liste vide */
        return list;
    }
    
    /* Parser JSON simple : on cherche les patterns "score": X et "nom": "Y" */
    char buffer[4096];
    size_t read = fread(buffer, 1, sizeof(buffer) - 1, f);
    fclose(f);
    buffer[read] = '\0';
    
    int idx = 0;
    const char* pos = buffer;
    
    while (idx < 5 && pos) {
        /* Chercher le prochain "score": */
        const char* score_pos = strstr(pos, "\"score\":");
        if (!score_pos) break;
        
        /* Parser le score */
        int score = 0;
        if (sscanf(score_pos, "\"score\": %d", &score) == 1) {
            list->scores[idx].score = score;
            
            /* Chercher le nom (valeur après le ':') */
            const char* nom_pos = strstr(score_pos, "\"nom\":");
            if (nom_pos) {
                const char* colon = strchr(nom_pos, ':');
                if (colon) {
                    /* Lire la chaîne entre guillemets suivant le ':' */
                    if (sscanf(colon, ": \"%49[^\"]\"", list->scores[idx].nom) != 1) {
                        list->scores[idx].nom[0] = '\0';
                    }
                }
            }
            
            idx++;
            pos = score_pos + 1;
        } else {
            break;
        }
    }
    
    list->nombre_scores = idx;
    return list;
}

void highscores_detruire(HighScoreList* list) {
    if (list) free(list);
}

int highscores_est_top5(HighScoreList* list, int score) {
    if (!list) return 0;
    
    /* Si on a moins de 5 scores, il rentre toujours */
    if (list->nombre_scores < 5) return 1;
    
    /* Sinon, il faut qu'il soit meilleur que le 5ème */
    return score > list->scores[4].score;
}

void highscores_inserer(HighScoreList* list, int score, const char* nom) {
    if (!list) return;
    
    /* Trouver la position pour insérer */
    int pos = list->nombre_scores;
    for (int i = 0; i < list->nombre_scores; ++i) {
        if (score > list->scores[i].score) {
            pos = i;
            break;
        }
    }
    
    /* Décaler les éléments après cette position */
    if (pos < 5) {
        for (int i = 4; i > pos; --i) {
            if (i - 1 >= 0 && i < 5) {
                list->scores[i] = list->scores[i - 1];
            }
        }
        
        /* Insérer le nouveau score */
        list->scores[pos].score = score;
        strncpy(list->scores[pos].nom, nom, 49);
        list->scores[pos].nom[49] = '\0';
        
        if (list->nombre_scores < 5) {
            list->nombre_scores++;
        }
    }
}

int highscores_sauvegarder(HighScoreList* list) {
    if (!list) return 0;
    
    /* Créer le répertoire data s'il n'existe pas */
    #ifdef _WIN32
        system("if not exist data mkdir data");
    #else
        system("mkdir -p data");
    #endif
    
    FILE* f = fopen(HIGHSCORES_FILE, "w");
    if (!f) return 0;
    
    /* Écrire le JSON */
    fprintf(f, "{\n");
    fprintf(f, "  \"highscores\": [\n");
    
    for (int i = 0; i < list->nombre_scores; ++i) {
        fprintf(f, "    {\n");
        fprintf(f, "      \"score\": %d,\n", list->scores[i].score);
        fprintf(f, "      \"nom\": \"%s\"\n", list->scores[i].nom);
        fprintf(f, "    }");
        
        if (i < list->nombre_scores - 1) {
            fprintf(f, ",");
        }
        fprintf(f, "\n");
    }
    
    fprintf(f, "  ]\n");
    fprintf(f, "}\n");
    
    fclose(f);
    return 1;
}
