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

void afficherwater(ALLEGRO_BITMAP *laby_texture){
    for(int i=0; i<15; i++){
        for (int j=0 ; j<27 ; j++){
            al_draw_tinted_scaled_rotated_bitmap_region(laby_texture,108, 0, 36, 36, al_map_rgb(255,255,255), 0, 0, 0+36*j*2, 0+36*i*2, 2, 2, 0, 0);
        }
    }
}

void afficherMaze(ALLEGRO_BITMAP *laby_texture, Lilypad lilypad[5], int frame_lilypad){
    for(int x=0 ; x<ROWS ; x++){
        for (int y=0 ; y<COLS ; y++){
            al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 0, 60, 72, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*x+LABY_WALL_SIZE, Y_PLATEAU+LABY_CASE_SIZE*y+LABY_WALL_SIZE, 1, 1, 0, 0);
            al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 0, 60, 72, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*(23-x)-LABY_WALL_SIZE, Y_PLATEAU+LABY_CASE_SIZE*y+LABY_WALL_SIZE, 1, 1, 0, 0);
        }
    }

    Coord current_red = {X_PLATEAU+LABY_CASE_SIZE*ROWS*2-LABY_WALL_SIZE , Y_PLATEAU+LABY_WALL_SIZE};
    Coord current_blue = {X_PLATEAU+LABY_WALL_SIZE , Y_PLATEAU+LABY_WALL_SIZE};

    for(int y = 0; y < ROWS; y++){ // on affiche les connexions entre tuiles (haut bas gauche droite)
        for(int x = 0; x < COLS; x++){
            current_blue.x = x*LABY_CASE_SIZE + X_PLATEAU+LABY_WALL_SIZE;
            current_blue.y = y*LABY_CASE_SIZE + Y_PLATEAU+LABY_WALL_SIZE;
            current_red.x = X_PLATEAU+LABY_CASE_SIZE*(ROWS*2+1)-LABY_WALL_SIZE - y*LABY_CASE_SIZE;
            current_red.y = x*LABY_CASE_SIZE + Y_PLATEAU+LABY_WALL_SIZE;
            if(x!=0 && (grid[y][x] == 4 || grid[y][x] == 8)){ // A GAUCHE
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 0, 24, 72, al_map_rgb(255,255,255), 0, 0, current_blue.x-LABY_WALL_SIZE, current_blue.y, 1, 1, 0, 0);
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 72, 60, 48, al_map_rgb(255,255,255), 0, 0, current_red.x, current_red.y-LABY_WALL_SIZE*2, 1, 1, 0, 0);
            }
            else if(x!=COLS-1 && (grid[y][x] == 2 || grid[y][x] == 6)){ // A DROITE
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 0, 24, 72, al_map_rgb(255,255,255), 0, 0, current_blue.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_blue.y, 1, 1, 0, 0);
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 72, 60, 48, al_map_rgb(255,255,255), 0, 0, current_red.x, current_red.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); 
            }
            else if(y!=0 && (grid[y][x] == 1 || grid[y][x] == 5)){ // EN HAUT
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 72, 60, 48, al_map_rgb(255,255,255), 0, 0, current_blue.x, current_blue.y-LABY_WALL_SIZE*2, 1, 1, 0, 0);
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 0, 24, 72, al_map_rgb(255,255,255), 0, 0, current_red.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_red.y, 1, 1, 0, 0);
            }
            else if(y!=ROWS-1 && (grid[y][x] == 3 || grid[y][x] == 7)){ // EN BAS
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 72, 60, 48, al_map_rgb(255,255,255), 0, 0, current_blue.x, current_blue.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); 
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 0, 24, 72, al_map_rgb(255,255,255), 0, 0, current_red.x-LABY_WALL_SIZE, current_red.y, 1, 1, 0, 0);
            }    
        }
    }

    for(int y = 0; y < ROWS; y++){ // On règle les problème de supperposition dans les angles en ajoutant des coins
        for(int x = 0; x < COLS; x++){ // Chaque coins a trois chances d'apparaitre selon la génération; par exemple pour l'angle gauche/haut : -la tuile gauche->tuile actuelle ET actuelle->haut OU actuelle->gauche ET haut->actuele OU haut->actuelle ET gauche->actuelle
            current_blue.x = x*LABY_CASE_SIZE + X_PLATEAU+LABY_WALL_SIZE;
            current_blue.y = y*LABY_CASE_SIZE + Y_PLATEAU+LABY_WALL_SIZE;
            current_red.x = X_PLATEAU+LABY_CASE_SIZE*(ROWS*2+1)-LABY_WALL_SIZE - y*LABY_CASE_SIZE;
            current_red.y = x*LABY_CASE_SIZE + Y_PLATEAU+LABY_WALL_SIZE;
            if(((grid[y][x] == 8) && (y!=0 && grid[y-1][x] == 7)) || ((x!=0 && grid[y][x-1] == 6) && (grid[y][x] == 5)) || ((x!=0 && grid[y][x-1] == 6) && (y!=0 && grid[y-1][x] == 7))){ // A GAUCHE ET EN HAUT
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 84, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_blue.x, current_blue.y-LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 96, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_red.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_red.y-LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
            }
            if(((grid[y][x] == 6) && (y!=0 && grid[y-1][x] == 7)) || ((grid[y][x] == 5) && (x != COLS-1 && grid[y][x+1] == 8)) || ((y!=0 && grid[y-1][x] == 7) && (x != COLS-1 && grid[y][x+1] == 8))){ // A DROITE ET EN HAUT
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 96, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_blue.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_blue.y-LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_red.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_red.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
            }
            if(((grid[y][x] == 8) && (y!=ROWS-1 && grid[y+1][x] == 5)) || ((x!=0 && grid[y][x-1] == 6) && (grid[y][x] == 7)) || ((y!=ROWS-1 && grid[y+1][x] == 5) && (x!=0 && grid[y][x-1] == 6))){ // A GAUCHE ET EN BAS
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 72, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_blue.x, current_blue.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 84, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_red.x, current_red.y-LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
            }
            if(((grid[y][x] == 6) && (y!=ROWS-1 && grid[y+1][x] == 5)) || ((grid[y][x] == 7) && (x != COLS-1 && grid[y][x+1] == 8)) || ((y!=ROWS-1 && grid[y+1][x] == 5) && (x != COLS-1 && grid[y][x+1] == 8))){ // A DROITE ET EN BAS
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 60, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_blue.x+LABY_CASE_SIZE-LABY_WALL_SIZE, current_blue.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
                al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 72, 72, 12, 36, al_map_rgb(255,255,255), 0, 0, current_red.x, current_red.y+LABY_CASE_SIZE-2*LABY_WALL_SIZE, 1, 1, 0, 0); // On pose une texture pour "cacher" la supperposition
            }
        }
    }

    for(int i = 0; i < 5; i ++){
        al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 120, 12, 12, 12, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*lilypad[i].x+LABY_WALL_SIZE*3, Y_PLATEAU+LABY_CASE_SIZE*lilypad[i].y+LABY_WALL_SIZE*3, 1, 1, 0, 0);
        al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 108, 36, 36, 36, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*lilypad[i].x+LABY_WALL_SIZE*2, Y_PLATEAU+LABY_CASE_SIZE*lilypad[i].y+LABY_WALL_SIZE*2, 1, 1, 0, 0);
        al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 108, 72, 36, 36, al_map_rgb(255,255,255), 17.5, 20.5, X_PLATEAU+LABY_CASE_SIZE*lilypad[i].x+LABY_WALL_SIZE*2+17.5, Y_PLATEAU+LABY_CASE_SIZE*lilypad[i].y+LABY_WALL_SIZE*2+20.5, 1, 1, M_PI_4*(int)(frame_lilypad/7.5)/2, 0);
    }

    al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 120, 96, 72, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*11-LABY_WALL_SIZE, Y_PLATEAU+LABY_CASE_SIZE*10+LABY_WALL_SIZE*1.6, 1, 1, 0, 0);
    al_draw_tinted_scaled_rotated_bitmap_region(laby_texture, 0, 120, 96, 72, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*12-LABY_WALL_SIZE*2, Y_PLATEAU+LABY_CASE_SIZE*10+LABY_WALL_SIZE*1.6, 1, 1, 0, 1);
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
    }if(y!=ROWS-1 && grid[y+1][x] == 0){ // EN BAS
        directions[nbDir] = 3;
        nbDir += 1;
    }if(x!=COLS-1 && grid[y][x+1] == 0){ // A DROITE
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
            grid[y-1][x] = 3;
            /* afficherMaze();
            al_rest(0.001); */
            expand(x, y-1);
            break;
        case 2:
            grid[y][x+1] = 4;
            /* afficherMaze();
            al_rest(0.001); */
            expand(x+1, y);
            break;
        case 3:
            grid[y+1][x] = 1;
            /* afficherMaze();
            al_rest(0.001); */
            expand(x, y+1);
            break;
        case 4:
            grid[y][x-1] = 2;
            /* afficherMaze();
            al_rest(0.001); */
            expand(x-1, y);
            break;
        }
        expand(x, y);
        
}

void generate_lilypad(Lilypad lilypad[5]){
    lilypad[0].x = getRandomInt(0, 4);
    lilypad[0].y = getRandomInt(0, 4);
    lilypad[1].x = getRandomInt(5, 10);
    lilypad[1].y = getRandomInt(0, 4);
    lilypad[2].x = getRandomInt(0, 4);
    lilypad[2].y = getRandomInt(5, 10);
    lilypad[3].x = getRandomInt(5, 10);
    lilypad[3].y = getRandomInt(5, 10);
    lilypad[4].x = getRandomInt(0, 10);
    lilypad[4].y = getRandomInt(0, 10);
}

void generate_maze(ALLEGRO_BITMAP* laby_texture, Lilypad lilypad[5], int frame_lilypad){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = 0;
        }
    }
    grid[0][0] = 9;
    expand(0,0);
    generate_lilypad(lilypad);
    afficherMaze(laby_texture, lilypad, frame_lilypad);
}

bool movePlayer(Player* player, int direction){
    switch (direction){
    case 1:
        if(player->y != 0 && (grid[player->y][player->x] == 5 || grid[player->y-1][player->x] == 7)){
            return true;
        }       
        break;
    case 2:
        if(player->x != COLS-1 && (grid[player->y][player->x] == 6 || grid[player->y][player->x+1] == 8)){
            return true;
        }  
        break;
    case 3:
        if(player->y != ROWS-1 && (grid[player->y][player->x] == 7 || grid[player->y+1][player->x] == 5)){
            return true;
        }
        break;
    case 4:
        if(player->x != 0 && (grid[player->y][player->x] == 8 || grid[player->y][player->x-1] == 6)){
            return true;
        } 
        break;
    }
    return false;
}

void stopMove(bool keystate[4], Animation animation, int direction){
    switch(direction){
    case 0:
        keystate[0] = false;
        animation.y = 0;   // IDLE FRONT
        animation.x = 24;
        break;
    case 1:
        keystate[1] = false;
        animation.y = 0;    // IDLE DROITE
        animation.x = 48;
        break;
    case 2:
        keystate[2] = false;
        animation.y = 0;    // IDLE BACK
        animation.x = 0;
        break;
    case 3:
        keystate[3] = false;
        animation.y = 0;    // IDLE GAUCHE
        animation.x = 48;
        break;
    }
}

#define PI 3.14159265

// Définition des constantes du pendule
const double g = 9.81;
const double L = 1.0;
const double m = 1.0;

// Fonction qui retourne la force apériodique f(t)
double force_ap(double t) {
    return 0.1 * sin(2 * PI * t);
}

// Fonction qui calcule la dérivée seconde de omega
double omega_dd(double omega, double omega_d, double t, double alpha) {
    double P = m * g;
    double f = force_ap(t);
    double f_d = -alpha * omega_d;
    double sin_omega = sin(omega);
    return (1.0 / L) * (P * sin_omega - f_d * omega_d - alpha * omega + f);
}

// Algorithme d'Euler
void euler(double omega[], double omega_d[], double t[], double alpha, double dt, int n) {
    for (int i = 0; i < n - 1; i++) {
        omega[i+1] = omega[i] + omega_d[i] * dt;
        omega_d[i+1] = omega_d[i] + omega_dd(omega[i], omega_d[i], t[i], alpha) * dt;
        t[i+1] = t[i] + dt;
    }
}

#endif