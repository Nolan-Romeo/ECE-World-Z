#include "std_iso.h"

int main(int argc, char* argv[]){

    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(1600, 900);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;

    int iterations = 0;
    while (!findrandomPath() && iterations < MAX_ITERATIONS) {
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
    //printf("Chemin trouve:\n");
    //printGrid();
    }

    while (1){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                al_destroy_display(disp);
                al_destroy_event_queue(queue);
                return 0;

            case ALLEGRO_EVENT_KEY_DOWN:
            
                switch (event.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        al_destroy_display(disp);
                        al_destroy_event_queue(queue);
                        return 0;

                    default:
                        break;
                } 
                break;
        
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            srand(0);

            al_clear_to_color(al_map_rgb(123, 125, 125));

            for(int x=0 ; x<10 ; x++){
                for (int y=0 ; y<10 ; y++){
                    al_draw_filled_rectangle(X_PLATEAU+60*x,Y_PLATEAU+60*y,X_PLATEAU+60*x+60,Y_PLATEAU+60*y+60,al_map_rgb(0+5*grid[x][y], 0, 0));
                }
            }

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}