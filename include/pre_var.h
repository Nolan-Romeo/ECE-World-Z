#ifndef pre_var_
#define pre_var_

#define CASE_SIZE 24
#define PLAT_SIZE 12
#define TUILE_SIZE_X 16
#define TUILE_SIZE_Y 18
#define TUILE_RESIZE 4
#define NB_ETAGE 6
#define MAX_ASTAR_ITERATIONS 600

#define X_PLATEAU (1600/2)-(60*22/2)
#define Y_PLATEAU (900/2)-((60*11)/2)

#define LABY_CASE_SIZE 60
#define LABY_WALL_SIZE 12

#define ROWS 11
#define COLS 11
#define MAX_ITERATIONS 1000

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

typedef struct{
    int x,y;
}Coord;

typedef struct{
    int x,y;
}Player;

#endif