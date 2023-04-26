#include "std_iso.h"

typedef struct {
    int position_x;
    int position_y;
    int statut;
    int visible;
}Taupe;

int main()
{
    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* fenetre = al_create_display(1920, 1080);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(fenetre));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    

    ALLEGRO_EVENT event;

    al_show_mouse_cursor(fenetre);

    ALLEGRO_BITMAP*fond = load_image("img/background.bmp");
    ALLEGRO_BITMAP*taupe_img =load_image("img/taupe.bmp");

    al_start_timer(timer);

    Taupe taupe[5] = { 10, 670, 4, 0,
                       230, 670, 4, 0,
                       600, 670, 4, 0,
                       1000, 670, 4, 0,
                       1600, 670, 4, 0
                    };
    
    bool redraw=true;
    int fin=0;
    int changePosition;

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
            else if(al_key_down(&keyboard_state, ALLEGRO_KEY_SPACE)) taupe->statut--;
        }
        if(redraw){

            changePosition = (changePosition >= 400)?0 : changePosition+1;

            al_draw_scaled_bitmap(fond,0,0,1920,1080,0,0,1920,1080,0);
                   
/*             if(changePosition == getRandomInt(30,60)){
                
                changePosition = 0;
            }

            if(changePosition %10 == 0){
                for (int i = 0; i < 6; i++)
                {
                    if(taupe[i].visible && taupe[i].statut > 0) taupe[i].statut -= 1;
                    if(taupe[i].statut == 0) taupe[i].statut=4;
                }     
            }*/
       
            for (int i = 0; i < 5; i++)
            {
                al_draw_tinted_scaled_rotated_bitmap_region(taupe_img, 103*taupe[i].statut, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, taupe[i].position_x, taupe[i].position_y, 3, 3, 0, 0);
            }

            int ordre[5] = {1,2,3,4,5};
            melangerTab(ordre, 5);

            for(int i = 0; i < 5; i++){
                taupe[i].visible = ordre[i];
            }
            
            /* int compare = 1;
            for (int j = 0; j < 5; j++)
            {   
                int i=0;
                while(compare != taupe[i].visible){
                    i++;
                }
                compare++;
                taupe[i].statut = changePosition/20;
            } */

            /* for(int i = 0; i < 5; i++){
                if((double)changePosition-(taupe[i].visible*60) <= 0){
                    taupe[i].statut = 4;
                }else {
                    taupe[i].statut = 4-changePosition/60.0-taupe[i].visible;
                }
                //taupe[i].statut = 4;
            } */
            
            
          
            al_flip_display();
        }
    }


    al_destroy_font(font);
    al_destroy_display(fenetre);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(fond);

    }
