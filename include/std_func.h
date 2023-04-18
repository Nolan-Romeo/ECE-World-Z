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

void afficherMaze(){
    for(int x=0 ; x<ROWS ; x++){
        for (int y=0 ; y<COLS ; y++){
            ALLEGRO_COLOR color = (grid[y][x] == 0)? al_map_rgb(0, 0, 0): (grid[y][x] <= 4)? al_map_rgb(0, 0, 255): al_map_rgb(0, 0, 255);
            al_draw_filled_rectangle(X_PLATEAU+60*x+10,Y_PLATEAU+60*y+10,X_PLATEAU+60*x+50,Y_PLATEAU+60*y+50,color);
            color = (grid[x][y] == 0)? al_map_rgb(0, 0, 0): (grid[x][y] <= 4)? al_map_rgb(0, 0, 255): al_map_rgb(0, 0, 255);
            al_draw_filled_rectangle(X_PLATEAU+60*(22-x)-10,Y_PLATEAU+60*y+10,X_PLATEAU+60*(22-x)-50,Y_PLATEAU+60*y+50,color);
        }
    }

    Coord current_red = {X_PLATEAU+60*22-10 , Y_PLATEAU+10};
    Coord current_blue = {X_PLATEAU+10 , Y_PLATEAU+10};

    for(int y = 0; y < 11; y++){
        for(int x = 0; x < 11; x++){
            current_blue.x = x*60 + X_PLATEAU+10;
            current_blue.y = y*60 + Y_PLATEAU+10;
            current_red.x = X_PLATEAU+60*22-10 - y*60;
            current_red.y = x*60 + Y_PLATEAU+10;
            ALLEGRO_COLOR color = (grid[y][x] == 0)? al_map_rgb(0, 0, 0): (grid[y][x] <= 4)? al_map_rgb(0, 0, 255): al_map_rgb(0, 0, 255);
            if( y!=0 && (grid[y][x] == 1 || grid[y][x] == 5)){ // EN HAUT
                al_draw_filled_rectangle(current_blue.x, current_blue.y-20, current_blue.x+40 , current_blue.y,color);
                al_draw_filled_rectangle(current_red.x+20, current_red.y, current_red.x , current_red.y+40,color);
            }
            else if(x!=0 && (grid[y][x] == 4 || grid[y][x] == 8)){ // A GAUCHE
                al_draw_filled_rectangle(current_blue.x-20, current_blue.y, current_blue.x , current_blue.y+40,color);
                al_draw_filled_rectangle(current_red.x, current_red.y-20, current_red.x-40 , current_red.y,color);
            }
            else if(y!=10 && (grid[y][x] == 3 || grid[y][x] == 7)){ // EN BAS
                al_draw_filled_rectangle(current_blue.x, current_blue.y+40, current_blue.x+40 , current_blue.y+60,color);
                al_draw_filled_rectangle(current_red.x-40, current_red.y, current_red.x-60 , current_red.y+40,color);
            }
            else if(x!=10 && (grid[y][x] == 2 || grid[y][x] == 6)){ // A DROITE
                al_draw_filled_rectangle(current_blue.x+40, current_blue.y, current_blue.x+60 , current_blue.y+40,color);
                al_draw_filled_rectangle(current_red.x, current_red.y+40, current_red.x-40 , current_red.y+60,color);
            }
        }
    }
    al_flip_display();
}

int choose_dir(int x, int y){
    int directions[4] = {0};
    int nbDir = 0;
    if( y!=0 && grid[y-1][x] == 0 ){ // EN HAUT
        directions[nbDir] = 1;
        nbDir += 1;
    }if(x!=0 && grid[y][x-1] == 0){ // A GAUCHE
        directions[nbDir] = 4;
        nbDir += 1;
    }if(y!=10 && grid[y+1][x] == 0){ // EN BAS
        directions[nbDir] = 3;
        nbDir += 1;
    }if(x!=10 && grid[y][x+1] == 0){ // A DROITE
        directions[nbDir] = 2;
        nbDir += 1;
    }

    if(nbDir == 0) return 0;
    return directions[getRandomInt(0, nbDir-1)];
}

void expand(int x, int y){
    int dir = choose_dir(x, y);
        switch (dir){
        case  0:
            grid[y][x] = (grid[y][x] == 0)? 9: grid[y][x]+4;
            return;
        case 1:
            grid[y][x] = 1;
            afficherMaze();
            al_rest(0.03);
            expand(x, y-1);
            break;
        case 2:
            grid[y][x] = 2;
            afficherMaze();
            al_rest(0.03);
            expand(x+1, y);
            break;
        case 3:
            grid[y][x] = 3;
            afficherMaze();
            al_rest(0.03);
            expand(x, y+1);
            break;
        case 4:
            grid[y][x] = 4;
            afficherMaze();
            al_rest(0.03);
            expand(x-1, y);
            break;
        }
        expand(x, y);
        
}

void generate_maze(){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = 0;
        }
    }
    expand(0,0);
    afficherMaze();
}

#endif