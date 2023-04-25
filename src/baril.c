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
    
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    ALLEGRO_DISPLAY* disp = al_create_display(WIDTH, HEIGHT);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //ALLEGRO_BITMAP *barrel = load_image("img/barrel.bmp");
    ALLEGRO_BITMAP *explosion = load_image("img/explosion.bmp");
    ALLEGRO_BITMAP *background = load_image("img/background.bmp");

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    int frame_explosion = 0;
    bool redraw = true;

    while (1){  

        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
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
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                al_destroy_display(disp);
                al_destroy_event_queue(queue);
                return 0;
                break;
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            frame_explosion = (frame_explosion>=39)?0: frame_explosion+1;

            al_clear_to_color(al_map_rgb(255, 255, 255));

            al_draw_bitmap(background,0,0,0);

            // EXPLOSION
            // al_draw_tinted_scaled_rotated_bitmap_region(explosion, 0+(frame_explosion/5)*64, 0, 64, 64, al_map_rgb(255,255,255), 0, 0, 500, 500, 2, 2, 0, 0);   

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}