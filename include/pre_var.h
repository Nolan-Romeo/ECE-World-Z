#ifndef pre_var_
#define pre_var_

#define WIDTH 1920
#define HEIGHT 1080

#define CASE_SIZE 24
#define PLAT_SIZE 12
#define TUILE_SIZE_X 16
#define TUILE_SIZE_Y 18
#define TUILE_RESIZE 4
#define NB_ETAGE 6
#define MAX_ASTAR_ITERATIONS 600

#define X_PLATEAU (WIDTH/2)-(LABY_CASE_SIZE*2*ROWS/2)
#define Y_PLATEAU (HEIGHT/2)-((LABY_CASE_SIZE*ROWS)/2)

#define LABY_CASE_SIZE 6
#define LABY_WALL_SIZE 1  

#define ROWS 110
#define COLS 110
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