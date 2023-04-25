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
    int frame_lilypad = 0;
    Animation animation = {0,0};
    bool keystate[4] = {false, false, false, false};
    int flip_img = 0;

    Player player1 = {0,0};

    Lilypad lilypad[5] = {{0,0, false}};

    int control_change = 0;

    int offsetX = 0, offsetY = 0;

    generate_maze(laby_texture, lilypad, frame_lilypad);

    while (1){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                if(keystate[0]){
                    flip_img = 0;
                    animation.y = 24*((frame/20)+1);
                    animation.x = 24;
                    offsetX = (offsetX < 20 && offsetX > -20) ? 0 : (offsetX < -40)? -60: (offsetX > 40)? 60: offsetX;
                    if((movePlayer(&player1, 1) || offsetY > 0) && offsetX == 0){
                        offsetY -= 2;
                    }    
                }
                if(keystate[3]){
                    flip_img = ALLEGRO_FLIP_HORIZONTAL;
                    animation.y = 24*((frame/20));
                    animation.x = 48;
                    offsetY = (offsetY < 20 && offsetY > -20) ? 0 : (offsetY < -40)? -60: (offsetY > 40)? 60: offsetY;
                    if((movePlayer(&player1, 4) || offsetX > 0) && offsetY == 0){
                        offsetX -= 2;
                    }        
                }
                if(keystate[2]){
                    flip_img = 0;
                    animation.y = 24*((frame/20)+1);
                    animation.x = 0;
                    offsetX = (offsetX < 20 && offsetX > -20) ? 0 : (offsetX < -40)? -60: (offsetX > 40)? 60: offsetX;
                    if((movePlayer(&player1, 3) || offsetY < 0) && offsetX == 0){
                        offsetY += 2;
                    }       
                }
                if(keystate[1]){
                    flip_img = 0;
                    animation.y = 24*((frame/20));
                    animation.x = 48;
                    offsetY = (offsetY < 20 && offsetY > -20) ? 0 : (offsetY < -40)? -60: (offsetY > 40)? 60: offsetY;
                    if((movePlayer(&player1, 2) || offsetX < 0) && offsetY == 0){
                        offsetX += 2;
                    }
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
                        keystate[control_change] = true;
                        keystate[(control_change+3)%4] = false;
                        keystate[(control_change+2)%4] = false;
                        keystate[(control_change+1)%4] = false;
                        break;
                    case ALLEGRO_KEY_Q:
                        frame = 0;
                        keystate[control_change] = false;
                        keystate[(control_change+3)%4] = true;
                        keystate[(control_change+2)%4] = false;
                        keystate[(control_change+1)%4] = false;
                        break;
                    case ALLEGRO_KEY_S:
                        frame = 0;
                        keystate[control_change] = false;
                        keystate[(control_change+3)%4] = false;
                        keystate[(control_change+2)%4] = true;
                        keystate[(control_change+1)%4] = false;
                        break;
                    case ALLEGRO_KEY_D:
                        frame = 0;
                        keystate[control_change] = false;
                        keystate[(control_change+3)%4] = false;
                        keystate[(control_change+2)%4] = false;
                        keystate[(control_change+1)%4] = true;
                        break;
                    default:
                        break;
                } 
                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode){
                case ALLEGRO_KEY_Z:
                    frame = 0;
                    stopMove(keystate, animation, (0+control_change)%4);
                    break;
                case ALLEGRO_KEY_Q:
                    frame = 0;
                    stopMove(keystate, animation, (3+control_change)%4);
                    break;
                case ALLEGRO_KEY_S:
                    frame = 0;
                    stopMove(keystate, animation, (2+control_change)%4);
                    break;                   
                case ALLEGRO_KEY_D:
                    frame = 0;
                    stopMove(keystate, animation, (1+control_change)%4);
                    break;
                default:
                    break;
                }
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            frame = (frame>=39)?0: frame+1;
            frame_lilypad = (frame_lilypad>=119)?0: frame_lilypad+1;   

            al_clear_to_color(al_map_rgb(0, 0, 0));

            afficherwater(laby_texture);
            afficherMaze(laby_texture, lilypad, frame_lilypad);

            if(offsetX >= 60){
                player1.x += 1;
                offsetX = 0;
            }
            if(offsetX <= -60){
                player1.x -= 1;
                offsetX = 0;
            }         
            if(offsetY >= 60){
                player1.y += 1;
                offsetY = 0;
            }
            if(offsetY <= -60){
                player1.y -= 1;
                offsetY = 0;
            }

            for(int i = 0; i < 5; i++){
                if(player1.x == lilypad[i].x && player1.y == lilypad[i].y && lilypad[i].used == false){
                    control_change += 1;
                    lilypad[i].used = true;
                    keystate[0] = false;
                    keystate[1] = false;
                    keystate[2] = false;
                    keystate[3] = false;
                }
            }


            //al_draw_filled_rectangle(X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_WALL_SIZE,X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_CASE_SIZE-LABY_WALL_SIZE,Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_CASE_SIZE-LABY_WALL_SIZE,al_map_rgb(255,0,0));

            al_draw_tinted_scaled_rotated_bitmap_region(character, 0+animation.x, 0+animation.y, 24, 24, al_map_rgb(255,255,255), 0, 0, X_PLATEAU+LABY_CASE_SIZE*player1.x+LABY_WALL_SIZE+6+offsetX, Y_PLATEAU+LABY_CASE_SIZE*player1.y+LABY_WALL_SIZE-6+offsetY, 2, 2, 0, flip_img);
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