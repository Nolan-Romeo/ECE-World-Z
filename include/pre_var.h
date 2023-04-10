#ifndef pre_var_
#define pre_var_

#define CASE_SIZE 24
#define PLAT_SIZE 12
#define TUILE_SIZE_X 16
#define TUILE_SIZE_Y 18
#define TUILE_RESIZE 4
#define NB_ETAGE 6
#define MAX_ASTAR_ITERATIONS 600

#define X_PLATEAU (1600/2)-(60*10/2)
#define Y_PLATEAU (900/2)-((60*10)/2)
#define X_PLATEAU_GRID (1600/2)-(55*10/2)
#define Y_PLATEAU_GRID (900/2)-(55*10/2)

#define ROWS 10
#define COLS 10
#define MAX_ITERATIONS 10000

int grid[ROWS][COLS];
int startRow = 0;
int startCol = 0;
int endRow = ROWS - 1;
int endCol = COLS - 1;
int maxLength = 30;

typedef struct Maillon{
    int xNext, yNext;
    struct Maillon *next;
}Maillon;

typedef struct{
    uint8_t type;
    uint8_t direction;
    int Gcoast, Hcoast, Fcoast; // G : direction du départ, H : direction de l'arrivée, F : G+H
    bool open;
    bool treated;
}Tile;

typedef struct{
    int x, y;
    int Fcoast, Hcoast;
}TileToExpand;

#endif