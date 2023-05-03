#include "std_iso.h"



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



    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(fenetre));
    al_register_event_source(queue, al_get_timer_event_source(timer));



    ALLEGRO_EVENT event;


    ALLEGRO_BITMAP*fond = load_image("img/background.bmp");
    ALLEGRO_BITMAP*taupe_img =load_image("img/taupe.bmp");
    ALLEGRO_BITMAP*marteau = load_image("img/hammer.bmp");
    //ALLEGRO_BITMAP*ecriture = load_image("img/things.png");
    //ALLEGRO_BITMAP*nombre = load_image("img/nb.png");
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_MOUSE_CURSOR*curseur = al_create_mouse_cursor(marteau,0,0);

    al_set_mouse_cursor(fenetre,curseur);

    Taupe taupe[5] = { {30, 650, 0},
                       {520, 650, 0},
                       {850, 650, 0},
                       {1150, 650, 0},
                       {1600, 650, 0}
                    }; 

    al_start_timer(timer);
    
    bool redraw=true;
    int fin=0;
    int frame_visible=0;
    int random = 0;
    int score=0;
    int limit_random = getRandomInt(120,180);

    while(!fin){

        if(!al_is_event_queue_empty(queue)){
            al_get_next_event(queue, &event);
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                gererTaupes(taupe);
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    al_destroy_display(fenetre);
                    al_destroy_event_queue(queue);
                    return 0;   
                default:
                    break;
                }
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (event.mouse.button == 1) { // Clic gauche
                    int x = event.mouse.x;
                    int y = event.mouse.y;
                    for(int i=0; i <5 ; i++){
                        if(taupe[i].visible){
                            if (x >= taupe[i].position_x && x <= taupe[i].position_x+103  && y>= 650 && y <= 650+97){
                                taupe[i].visible=0;
                                score=score+1;
                            }
                        }
                    }
                    
                }
                break;

            default:
                break;
            }
        }

        if(redraw){
            if(random == limit_random){
                limit_random = getRandomInt(600,800);
                random = 0;
                random_taupe(taupe);
            }else{
                random++;
            }

            al_draw_scaled_bitmap(fond,0,0,1920,1080,0,0,1920,1080,0);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 100, ALLEGRO_ALIGN_CENTER, "Mon score : %d", score);

            //al_draw_scaled_bitmap(ecriture,0,0,800,172,0,0,800,172,0);
            //al_draw_tinted_scaled_rotated_bitmap_region(nombre, 0+44*(score), 0, 103, 100, al_map_rgb(255,255,255), 0, 0, 750, 20, 2, 2, 0, 0);

            afficher_taupe(taupe,taupe_img,&frame_visible);          

            al_flip_display();
        }
    }

    al_destroy_bitmap(marteau);
    al_destroy_mouse_cursor(curseur);
    al_destroy_timer(timer);
    al_destroy_bitmap(fond);
    al_destroy_bitmap(taupe_img);
    //al_destroy_bitmap(ecriture);
    al_destroy_font(font);

    }