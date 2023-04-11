#ifndef map_
#define map_

#include "std_iso.h"

#define MAX_LINE_LENGTH 256

// Cette fonction lit une ligne du fichier jusqu'à ce qu'elle atteigne la fin de ligne ou la fin de fichier.
// Elle renvoie le nombre de caractères lus ou -1 en cas d'erreur.
int read_line(FILE *file, char *buffer, int max_length) {
    int i = 0;
    char c = fgetc(file);
    while (c != EOF && c != '\n' && i < max_length - 1) {
        buffer[i] = c;
        i++;
        c = fgetc(file);
    }
    buffer[i] = '\0';
    if (c == EOF && i == 0) {
        return -1;
    }
    return i;
}

// Cette fonction prend une ligne de texte et la découpe en plusieurs chaînes de caractères en utilisant la virgule comme séparateur.
// Elle renvoie le nombre de chaînes de caractères trouvées.
int parse_line(char *line, char **strings, int max_strings) {
    int i = 0;
    char *token = strtok(line, ",");
    while (token != NULL && i < max_strings) {
        strings[i] = token;
        i++;
        token = strtok(NULL, ",");
    }
    return i;
}

// Cette fonction extrait toutes les chaînes de caractères séparées par une virgule dans le fichier spécifié.
// Elle renvoie le nombre de chaînes de caractères trouvées.
int extract_strings_from_file(const char *filename, char **strings, int max_strings) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }
    char line[MAX_LINE_LENGTH];
    int total_strings = 0;
    while (read_line(file, line, MAX_LINE_LENGTH) != -1) {
        total_strings += parse_line(line, &strings[total_strings], max_strings - total_strings);
        if (total_strings >= max_strings) {
            break;
        }
    }
    fclose(file);
    return total_strings;
}

void saveMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "w");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");
    for(int z = 0; z < NB_ETAGE; z++){
        for(int y = 0; y < PLAT_SIZE; y++){
            for(int x = 0; x < PLAT_SIZE; x++){
                fprintf(pMap, "%c", tab[z][y][x].type);
            }
            fprintf(pMap, ",");
        }
        fprintf(pMap, "\n");
    }
    fclose(pMap);
    pMap = NULL;
}

void loadMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "r");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");

    char* buffer = (char*) malloc(1024*sizeof(char));
    
    read_line(pMap, buffer, 1024);

    printf("ligne : %s", buffer);

    fclose(pMap);
    pMap = NULL;
}

void resetMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "w");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");
    for(int z = 0; z < NB_ETAGE; z++){
        for(int y = 0; y < PLAT_SIZE; y++){
            for(int x = 0; x < PLAT_SIZE; x++){
                tab[z][y][x].type = 0;
                fprintf(pMap, "%c", 0);
            }
        }
    }
    fclose(pMap);
    pMap = NULL;
}

#endif