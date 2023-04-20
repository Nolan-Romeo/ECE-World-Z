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

    ALLEGRO_BITMAP  *laby_texture = load_image("img/Labyrinthe.bmp");
    ALLEGRO_BITMAP  *character = load_image("img/character.bmp");

    bool redraw = true;

    int frame = 0;
    Animation animation = {0,0};
    Keystate keystate = {false, false, false, false};
    int flip_img = 0;

    Player player1 = {0,0};

    generate_maze(laby_texture);

    while (1){

        frame = (frame>=59)?0: frame+1;

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                if(keystate.z){
                    animation.y = 24*((frame/30)+1);
                    animation.x = 24;                    
                }
                if(keystate.q){
                    animation.y = 24*((frame/30)+1);
                    animation.x = 48;                    
                }
                if(keystate.s){
                    animation.y = 24*((frame/30)+1);
                    animation.x = 0;                    
                }
                if(keystate.d){
                    animation.y = 24*((frame/30)+1);
                    animation.x = 48;
                }
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
                        frame = 0;
                        flip_img = 0;
                        keystate.z = true;
                        movePlayer(&player1, 1);
                        break;
                    case ALLEGRO_KEY_Q:
                        frame = 0;
                        flip_img = ALLEGRO_FLIP_HORIZONTAL;
                        keystate.q = true;
                        movePlayer(&player1, 4);
                        break;
                    case ALLEGRO_KEY_S:
                        frame = 0;
                        flip_img = 0;
                        keystate.s = true;
                        movePlayer(&player1, 3);
                        break;
                    case ALLEGRO_KEY_D:
                        frame = 0;
                        flip_img = 0;
                        keystate.d = true;
                        movePlayer(&player1, 2);
                        break;
                    default:
                        break;
                } 
                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode){
                case ALLEGRO_KEY_Z:
                    frame = 0;
                    keystate.z = false;
                    animation.y = 0;   // IDLE FRONT
                    animation.x = 24;
                    break;
                case ALLEGRO_KEY_Q:
                    frame = 0;
                    keystate.q = false;
                    animation.y = 0;    // IDLE GAUCHE
                    animation.x = 48;
                    break;
                case ALLEGRO_KEY_S:
                    frame = 0;
                    keystate.s = false;
                    animation.y = 0;    // IDLE BACK
                    animation.x = 0; 
                    break;                   
                case ALLEGRO_KEY_D:
                    frame = 0;
                    keystate.d = false;
                    animation.y = 0;    // IDLE DROITE
                    animation.x = 48; 
                    break;
                
                default:
                    break;
                }
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(0, 0, 0));

            //al_draw_rectangle(1600/2,0,1600/2,900,al_map_rgb(0,255,0),1);
            //al_draw_rectangle(0,900/2,1600,900/2,al_map_rgb(0,255,0),1);

            afficherMaze(laby_texture);

            //al_draw_filled_rectangle(X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_WALL_SIZE,X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_CASE_SIZE-LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_CASE_SIZE-LABY_WALL_SIZE,al_map_rgb(255,0,0));

            al_draw_tinted_scaled_rotated_bitmap_region(character, 0+animation.x, 0+animation.y, 24, 24, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_WALL_SIZE+6, Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_WALL_SIZE-6, 2, 2, 0, flip_img);
            //(frame == 59)?printf("%d | \n",frame):printf("%d | ",frame);
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}