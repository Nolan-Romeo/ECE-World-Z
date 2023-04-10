#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 10
#define COLS 10
#define MAX_ITERATIONS 10000

int grid[ROWS][COLS];
int startRow = 0;
int startCol = 0;
int endRow = ROWS - 1;
int endCol = COLS - 1;
int maxLength = 40;

void printGrid() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

bool findPath() {
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

int main() {
    srand(time(NULL));
    int iterations = 0;
    while (!findPath() && iterations < MAX_ITERATIONS) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                grid[i][j] = 0;
            }
        }
        iterations++;
    }
    if (iterations == MAX_ITERATIONS) {
        printf("Pas de chemin trouve.\n");
        } else {
    printf("Chemin trouve:\n");
    printGrid();
    }
    return 0;
}

