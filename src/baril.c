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

    ALLEGRO_BITMAP *barrel = load_image("img/barrel.bmp");
    ALLEGRO_BITMAP *explosion = load_image("img/explosion.bmp");
    ALLEGRO_BITMAP *background = load_image("img/background.bmp");
    ALLEGRO_BITMAP *chain = load_image("img/chain.bmp");
    ALLEGRO_BITMAP *liane = load_image("img/liane.bmp");

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    bool redraw = true;
    
    int frame = 0;
    int frame_explosion = 0;

    int barrel_count = 2;

    double acc = 9.81*55;
    double vit = 0;
    double pos = 48;

    bool start = false;
    bool stop = false;

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
                    case ALLEGRO_KEY_S:
                        start = true;
                        break;
                    case ALLEGRO_KEY_SPACE:
                        stop = true;
                        break;  
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

            if(start){
                barrel_count = (pos-48)/32+1;
                frame = frame+1;
                if(!stop){
                    vit = acc*(frame/60.0);
                    pos = (vit*(frame/60.0))/2+48;
                }

                for (int i = -1; i < barrel_count; i++) // RAJOUTER UN COMPTEUR
                {
                    al_draw_bitmap(chain,860,pos-32*i,0);
                    al_draw_bitmap(chain,1020,pos-32*i,0);
                }

                al_draw_bitmap(barrel,816, pos+64,0);
                al_draw_bitmap(barrel,816+160, pos+64,0);
            }
            //al_draw_tinted_scaled_rotated_bitmap_region(explosion, 0+(frame_explosion/5)*64, 0, 64, 64, al_map_rgb(255,255,255), 0, 0, 500, 500, 2, 2, 0, 0);   

            al_draw_bitmap(liane,830,30,0);
            al_draw_bitmap(liane,830+128+32,30,ALLEGRO_FLIP_HORIZONTAL);

            //al_draw_line(960,0,960,1080,al_map_rgb(0,255, 0),1);
            //al_draw_line(0,1080/2,1920,1080/2,al_map_rgb(0,255, 0),1);

            al_flip_display();

            printf("%f | ",pos);

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}