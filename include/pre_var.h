#ifndef pre_var_
#define pre_var_

#define CASE_SIZE 24
#define PLAT_SIZE 12
#define TUILE_SIZE_X 16
#define TUILE_SIZE_Y 18
#define TUILE_RESIZE 2
#define NB_ETAGE 6
#define MAX_ASTAR_ITERATIONS 600

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

typedef struct {
    int position_x;
    int position_y;
    int statut;
    int visible;
}Taupe;


#endif