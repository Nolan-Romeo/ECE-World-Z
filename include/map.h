#ifndef map_
#define map_

#include "std_iso.h"

void saveMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen(map, "w");
    for(int z = 0; z < NB_ETAGE; z++){
        for(int y = 0; y < PLAT_SIZE; y++){
            for(int x = 0; x < PLAT_SIZE; x++){
                if(pMap == NULL) printf("erreur d'ouverture de fichier");
                fprintf(pMap, "%c", tab[z][y][x].type);
            }
        }
    }
    fclose(pMap);
    pMap = NULL;
}

void loadMap(Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE], char* map){
    FILE* pMap = NULL;
    pMap = fopen("./map", "r");
    if(pMap == NULL) printf("erreur d'ouverture de fichier");
    
    int i = 0;
    while(i < (NB_ETAGE*PLAT_SIZE*PLAT_SIZE) && fscanf(pMap, "%c", &tab[i/(PLAT_SIZE*PLAT_SIZE)][(i/PLAT_SIZE)%PLAT_SIZE][i%(PLAT_SIZE)].type) == 1) {
        i += 1;
    }
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