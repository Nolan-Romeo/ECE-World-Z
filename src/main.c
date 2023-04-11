#include "std_iso.h"

Tile tab[NB_ETAGE][PLAT_SIZE][PLAT_SIZE] = { [0 ... NB_ETAGE-1][0 ... PLAT_SIZE-1][0 ... PLAT_SIZE-1] = {0,0,-1,-1,-1,false,false} };

int main(int argc, char* argv[])
{
    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 20.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(1600, 900);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;

    ALLEGRO_BITMAP* map = load_image("img/tinyBlocks.bmp");
    al_start_timer(timer);

    int etage = 0;

    int brush = 1;

    int xSel = -1, ySel = -1;

    loadMap(tab, "./map");

    Maillon* path = NULL;

    int playerX = 6, playerY = 6;

    while(1)
    {
        ALLEGRO_MOUSE_STATE mouse_state;
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            ALLEGRO_KEYBOARD_STATE keyboard_state;
            al_get_keyboard_state(&keyboard_state);
            if(al_key_down(&keyboard_state, ALLEGRO_KEY_UP)){
                if(etage < NB_ETAGE-1) etage += 1;
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)){
                if(etage > 0) etage -= 1;
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_S)){
                saveMap(tab, "./map");
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_L)){
                loadMap(tab, "./map");
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_O)){
                resetMap(tab, "./map");
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_PAD_PLUS)){
                brush = (brush < 90)? brush+1: brush;
            }else if(al_key_down(&keyboard_state, ALLEGRO_KEY_MINUS) || al_key_down(&keyboard_state, ALLEGRO_KEY_PAD_MINUS)){
                brush = (brush > 1)? brush-1: 1;
            }
            else if(al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) break;
        }
        else if((event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)){
            al_get_mouse_state(&mouse_state);
            for(int x = 0; x < PLAT_SIZE; x++){
                for(int y = 0; y < PLAT_SIZE; y++){
                    if(mouse_state.x > x*CASE_SIZE && mouse_state.x < (x+1)*CASE_SIZE && 
                       mouse_state.y > y*CASE_SIZE && mouse_state.y < (y+1)*CASE_SIZE){
                        if(mouse_state.buttons == 1) tab[etage][y][x].type = (tab[etage][y][x].type == 0)? brush: tab[etage][y][x].type;
                        else if(mouse_state.buttons == 2) tab[etage][y][x].type = 0;
                    }
                }
            }
        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            srand(0);

            al_draw_rectangle(2, 2, PLAT_SIZE*CASE_SIZE+2, PLAT_SIZE*CASE_SIZE+2, al_map_rgb(255,255,255), 2);
            al_draw_textf(font, al_map_rgb(255,255,255), PLAT_SIZE*CASE_SIZE+10, 10, 0, "etage %d", etage);
            al_draw_textf(font, al_map_rgb(255,255,255), PLAT_SIZE*CASE_SIZE+10, 20, 0, "brush %d", brush);
            al_draw_tinted_scaled_rotated_bitmap_region(map, 0+(((brush-1)%9)*TUILE_SIZE_X), 0+(((brush-1)/9)*TUILE_SIZE_Y), TUILE_SIZE_X, TUILE_SIZE_Y, al_map_rgb(255,255,255), 0, 0, PLAT_SIZE*CASE_SIZE+10, 30, 3, 3, 0, 0);

            int xOffset = 0;
            int yOffset = 0;

            int grilleSizeX = 64;
            int grilleSizeY = 32;

            ALLEGRO_COLOR selection_mask = al_get_pixel(map, 144+(mouse_state.x%grilleSizeX)/4, 0+(mouse_state.y%grilleSizeY)/4);

            int xSelIso = (mouse_state.x-832+6*64)/grilleSizeX*2-12;
            int ySelIso = (mouse_state.y-256)/grilleSizeY*2;

            if (selection_mask.r == 1.0f && selection_mask.g == 1.0f){
                xOffset += 8*TUILE_RESIZE;
                yOffset += 4*TUILE_RESIZE;
                xSelIso += 1;
                ySelIso += 1;
            } else if (selection_mask.g >= 0.5f){
                xOffset -= 8*TUILE_RESIZE;
                yOffset += 4*TUILE_RESIZE;
                xSelIso -= 1;
                ySelIso += 1;
            } else if (selection_mask.b >= 0.5f){
                xOffset -= 8*TUILE_RESIZE;
                yOffset -= 4*TUILE_RESIZE;
                xSelIso -= 1;
                ySelIso -= 1;
            } else if (selection_mask.r >= 0.5f){
                xOffset += 8*TUILE_RESIZE;
                yOffset -= 4*TUILE_RESIZE;
                xSelIso += 1;
                ySelIso -= 1;
            }

            xSel = (((abs(ySelIso)+abs(xSelIso)) >= 0) && ((abs(ySelIso)+abs(xSelIso)) <= PLAT_SIZE*2) && (ySelIso+xSelIso)/2 >= 0 && (ySelIso-xSelIso)/2 >= 0 && tab[0][(ySelIso-xSelIso)/2][(ySelIso+xSelIso)/2].type != 0 && tab[1][(ySelIso-xSelIso)/2][(ySelIso+xSelIso)/2].type == 0)? (ySelIso+xSelIso)/2: xSel;
            ySel = (((abs(ySelIso)+abs(xSelIso)) >= 0) && ((abs(ySelIso)+abs(xSelIso)) <= PLAT_SIZE*2) && (ySelIso-xSelIso)/2 >= 0 && (ySelIso+xSelIso)/2 >= 0 && tab[0][(ySelIso-xSelIso)/2][(ySelIso+xSelIso)/2].type != 0 && tab[1][(ySelIso-xSelIso)/2][(ySelIso+xSelIso)/2].type == 0)? (ySelIso-xSelIso)/2: ySel;

            //printf("%d, %d \n", xSel, ySel);

            if ((playerX != xSel || playerY != ySel) && path == NULL){
                if(!findPath(playerX, playerY, xSel, ySel, tab, &path)){
                    //printf("Erreur chemin impossible\n");
                }
            } else if(path != NULL){
                playerX = path->xNext;
                playerY = path->yNext;
                LSCRemoveStart(&path);
            }

            //if(path->next != NULL)printf("%d %d\n", path->next->xNext, path->next->yNext);

            //printf("%d %d \n", xSel, ySel);

            for(int z = 0; z < NB_ETAGE; z++){
                for(int x = 0; x < PLAT_SIZE; x++){
                    for(int y = 0; y < PLAT_SIZE; y++){
                        if(tab[z][y][x].type != 0) {
                            int typeTuileX = (tab[z][y][x].type-1) % 9;
                            int typeTuileY = (tab[z][y][x].type-1) / 9;
                            if(z == etage) al_draw_rectangle(x*CASE_SIZE+2, y*CASE_SIZE+2, x*CASE_SIZE+CASE_SIZE+2, y*CASE_SIZE+CASE_SIZE+2, al_get_pixel(map, typeTuileX*TUILE_SIZE_X+TUILE_SIZE_X/2, typeTuileY*TUILE_SIZE_Y+4), 2);
                            al_draw_tinted_scaled_rotated_bitmap_region(map, 0+(typeTuileX*TUILE_SIZE_X), 0+(typeTuileY*TUILE_SIZE_Y), TUILE_SIZE_X, TUILE_SIZE_Y, al_map_rgb(255,255,255), 0, 0, (((x-z)*(TUILE_SIZE_X))-((y-z)*(TUILE_SIZE_X))-2+416)*TUILE_RESIZE/2, 0+(((y-z)*8)+((x-z)*8)+128-(4*z))*TUILE_RESIZE/2, TUILE_RESIZE, TUILE_RESIZE, 0, 0);
                        }
                        if(playerX >= 0 && playerY >= 0 && tab[0][ySel][xSel].type != 0 && tab[1][ySel][xSel].type == 0 && z == 0) al_draw_tinted_scaled_rotated_bitmap_region(map, 144, TUILE_SIZE_Y, TUILE_SIZE_X, 9, al_map_rgb(255,255,255), 0, 0, ((playerX*(TUILE_SIZE_X))-(playerY*(TUILE_SIZE_X))-2+416)*TUILE_RESIZE/2, 0+((playerY*8)+(playerX*8)+128)*TUILE_RESIZE/2, TUILE_RESIZE, TUILE_RESIZE, 0, 0); // affiche la tuile séléctionnée
                    }
                }
            }

            //al_draw_rectangle((mouse_state.x/grilleSizeX)*grilleSizeX+xOffset, (mouse_state.y/grilleSizeY)*grilleSizeY+yOffset, (mouse_state.x/grilleSizeX+1)*grilleSizeX+xOffset, (mouse_state.y/grilleSizeY+1)*grilleSizeY+yOffset, al_map_rgb(255,0,0), 2);
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}