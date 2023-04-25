#include "std_iso.h"

typedef struct 
{
    int position_x;
    int position_y;
    int image;
};

int main()
{
    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 20.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* fenetre = al_create_display(1920, 1080);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(fenetre));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;

    al_show_mouse_cursor(fenetre);

    ALLEGRO_BITMAP*fond = load_image("img/carte.png");
    ALLEGRO_BITMAP*taupe=load_image("img/taupe.png");

    al_start_timer(timer);
    
    bool redraw=true;
    int fin=0;
    int image=4;

    while(!fin){

        al_wait_for_event(queue, &event);
        

         if (event.type == ALLEGRO_EVENT_TIMER && al_is_event_queue_empty(queue)) {
            redraw=true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            fin = 1;
        }else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            ALLEGRO_KEYBOARD_STATE keyboard_state;
            al_get_keyboard_state(&keyboard_state);
            if(al_key_down(&keyboard_state, ALLEGRO_KEY_ESCAPE)) fin=1;
            else if(al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE)) image--;
        }
        if(redraw){

            al_draw_scaled_bitmap(fond,0,0,612,404,0,0,1920,1080,0);

            switch (getRandomInt(0,4))
            {
            case 0 :
                al_draw_tinted_scaled_rotated_bitmap_region(taupe, 103*image, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, 500, 500, 3, 3, 0, 0);
                break;
            case 1 :
                al_draw_tinted_scaled_rotated_bitmap_region(taupe, 103*image, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, 1000, 500, 3, 3, 0, 0);
                break;
            case 2 : 
                al_draw_tinted_scaled_rotated_bitmap_region(taupe, 103*image, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, 400, 700, 3, 3, 0, 0);
                break;
            case 3 :
                al_draw_tinted_scaled_rotated_bitmap_region(taupe, 103*image, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, 800, 700, 3, 3, 0, 0);
                break;
            case (4) :
                al_draw_tinted_scaled_rotated_bitmap_region(taupe, 103*image, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, 1200, 700, 3, 3, 0, 0);
                break;
            default:
                break;
            }

            
            
            
            
            
            
            al_flip_display();
        }
    }


    al_destroy_font(font);
    al_destroy_display(fenetre);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(fond);

    }
