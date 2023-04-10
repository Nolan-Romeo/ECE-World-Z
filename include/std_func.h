#ifndef std_func_
#define std_func_

#define load_image(args...) load_image_macro(args, __LINE__, __FILE__, __func__)

#include "std_iso.h"

void strgen(char *chaine_source, char *chaine_dest, int n) {
    strncpy(chaine_dest, chaine_source, n);
    chaine_dest[n] = '\0';
}

int getRandomInt(int min, int max){
    int i = (rand() % (max-min+1)) + min;
    return i;
}

ALLEGRO_BITMAP* load_image_macro(char* path, int line, const char* file, const char* func){
    ALLEGRO_BITMAP* img = al_load_bitmap(path);
    if(img == NULL) {
        printf("\033[0;31m");
        fprintf(stderr, "Erreur chargement image ");
        printf("\033[0;36m");
        fprintf(stderr, "(ligne %d, file : %s, fonction : %s)\n", line, file, func);
        printf("\033[0;m");
        exit(0);
    }
    return img;
}

double degToRad(double deg){
	return deg*M_PI / 180;
}

void LSCAddStart(Maillon** maillon, int x, int y){
    if(*maillon == NULL){ // si la liste est vide on met à la première position
        *maillon = (Maillon*) malloc(sizeof(Maillon));
        (*maillon)->next = NULL;
        (*maillon)->xNext = x;
        (*maillon)->yNext = y;
    } else { // sinon, on décale et on met au début
        Maillon* tmp = (Maillon*) malloc(sizeof(Maillon));
        tmp->xNext = x;
        tmp->yNext = y;
        tmp->next = (*maillon);
        *maillon = tmp;
        tmp = NULL;
    }
}

void LSCRemoveStart(Maillon** maillon){
    if(*maillon == NULL){ // si la liste est vide on dit impossible
        printf("\033[0;31m");
        fprintf(stderr, "Erreur liste vide\n");
        printf("\033[0;36m");
        exit(0);
    }else {
        Maillon* tmp = (*maillon)->next;
        free(*maillon);
        *maillon = tmp;
        tmp = NULL;
    }
}

bool inGrid(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

bool isVisited(int row, int col) {
    return grid[row][col] != 0;
}

bool isEnd(int row, int col) {
    return row == endRow && col == endCol;
}

bool isMaxLength(int length) {
    return length == maxLength;
}

int getRandomDirection() {
    return rand() % 4; // 0 = haut, 1 = bas, 2 = gauche, 3 = droite
}

void move(int *row, int *col, int direction) {
    switch (direction) {
        case 0: // haut
            (*row)--;
            break;
        case 1: // bas
            (*row)++;
            break;
        case 2: // gauche
            (*col)--;
            break;
        case 3: // droite
            (*col)++;
            break;
    }
}

bool findrandomPath() {
    int currentRow = startRow;
    int currentCol = startCol;
    int currentLength = 1;
    grid[currentRow][currentCol] = currentLength;
    int iterations = 0;
    while (!isEnd(currentRow, currentCol) && !isMaxLength(currentLength) && iterations < MAX_ITERATIONS) {
        int direction = getRandomDirection();
        int newRow = currentRow;
        int newCol = currentCol;
        move(&newRow, &newCol, direction);
        if (inGrid(newRow, newCol) && !isVisited(newRow, newCol)) {
            currentLength++;
            grid[newRow][newCol] = currentLength;
            currentRow = newRow;
            currentCol = newCol;
        }
        iterations++;
    }
    return isEnd(currentRow, currentCol);
}

void printGrid() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

#endif