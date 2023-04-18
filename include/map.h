#ifndef map_
#define map_

#include "std_iso.h"

#define MAX_LINE_LENGTH (2*PLAT_SIZE*PLAT_SIZE)

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

void saveMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "w");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");
    for(int z = 0; z < NB_ETAGE; z++){
        for(int y = 0; y < PLAT_SIZE; y++){
            for(int x = 0; x < PLAT_SIZE; x++){
                fprintf(pMap, "%c", tab[z][y][x].type+1);
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

    char buffer[MAX_LINE_LENGTH] = {0};

    int x = 0, y = 0;
    for(int z = 0; z < NB_ETAGE; z++){
        int lenght = read_line(pMap, buffer, MAX_LINE_LENGTH);
        //printf("ligne : %s \n", buffer);
        for(int index = 0; index < lenght; index++){
            if(buffer[index] == ',' || x > PLAT_SIZE){
                y++;
                x = 0;
            }
            else {
                tab[z][y][x].type = buffer[index]-1;
                x++;
            }
        }
        y = 0;
    }
    

    fclose(pMap);
    pMap = NULL;
}

void resetMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "w+");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");
    for(int z = 0; z < NB_ETAGE; z++){
        for(int y = 0; y < PLAT_SIZE; y++){
            for(int x = 0; x < PLAT_SIZE; x++){
                tab[z][y][x].type = 0;
                fprintf(pMap, "%c", 1);
            }
            fprintf(pMap, ",");
        }
        fprintf(pMap, "\n");
    }
    fclose(pMap);
    pMap = NULL;
}

#endif