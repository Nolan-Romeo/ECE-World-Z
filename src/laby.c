#include "std_iso.h"

int main(int argc, char* argv[]){

    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(WIDTH, HEIGHT);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    ALLEGRO_BITMAP* laby_texture = load_image("img/Labyrinthe.bmp");

    bool redraw = true;

    Player player1 = {0,0};

    generate_maze(laby_texture);

    while (1){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                al_destroy_display(disp);
                al_destroy_event_queue(queue);
                return 0;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
            
                switch (event.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        al_destroy_display(disp);
                        al_destroy_event_queue(queue);
                        return 0;
                    case ALLEGRO_KEY_Z:
                        movePlayer(&player1, 1);
                        printf(" Player %d:%d \n", player1.x, player1.y);
                        break;
                    case ALLEGRO_KEY_Q:
                        movePlayer(&player1, 4);
                        printf(" Player %d:%d \n", player1.x, player1.y);
                        break;
                    case ALLEGRO_KEY_S:
                        movePlayer(&player1, 3);
                        printf(" Player %d:%d \n", player1.x, player1.y);
                        break;
                    case ALLEGRO_KEY_D:
                        movePlayer(&player1, 2);
                        printf(" Player %d:%d \n", player1.x, player1.y);
                        break;
                    default:
                        break;
                } 
                break;
        
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(0, 0, 0));

            //al_draw_rectangle(1600/2,0,1600/2,900,al_map_rgb(0,255,0),1);
            //al_draw_rectangle(0,900/2,1600,900/2,al_map_rgb(0,255,0),1);

            afficherMaze(laby_texture);

            al_draw_filled_rectangle(X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_WALL_SIZE,X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_CASE_SIZE-LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_CASE_SIZE-LABY_WALL_SIZE,al_map_rgb(255,0,0));

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}