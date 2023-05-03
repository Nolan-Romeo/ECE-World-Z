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

void melangerTab(int* tab, size_t tailleTab){
    int j; //position avec laquelle notre valeur va être échangée
    uint8_t buffer;
    for(int i = 0; i < tailleTab; i++){ //on parcours le tableau pour échanger la valeur d'une case avec une autre
        j = getRandomInt(0,tailleTab-1); //on génère l'indice de la case avec laquelle notre valeur est échangée
        buffer = tab[j]; //on mémorise la valeur pour ne pas la perdre
        tab[j] = tab[i]; //on met i dans j
        tab[i] = buffer; //on met j dans i
    }
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



void afficher_taupe(Taupe taupe[],ALLEGRO_BITMAP*taupe_img, int *frame){
	*frame=(*frame == 0)?200:*frame-1;
	for(int i=0; i<5; i++){
        if(taupe[i].visible){
            al_draw_tinted_scaled_rotated_bitmap_region(taupe_img, 0+103*(*frame/40), 97, 103, 100, al_map_rgb(255,255,255), 0, 0, taupe[i].position_x, taupe[i].position_y, 3, 3, 0, 0);
        }
        else{
            al_draw_tinted_scaled_rotated_bitmap_region(taupe_img, 103*4, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, taupe[i].position_x, taupe[i].position_y, 3, 3, 0, 0);
        } 
	}

}       

void gererTaupes(Taupe taupe[]){
    for(int j=0; j<5; j++){
        if(taupe[j].visible > 0){
    	    taupe[j].visible--;
        }
    }
}

void random_taupe(Taupe taupe[]){
    int i= getRandomInt(0,4);
    for(int j=i; j<i+5; j++){
        if(taupe[j%5].visible==0){
            taupe[i].visible=getRandomInt(400,600);
            break;
        }
    }
}




#endif