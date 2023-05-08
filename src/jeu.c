#include "std_iso.h"



int main()
{
    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();

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
    ALLEGRO_BITMAP*ecriture = load_image("img/indi.png");
    ALLEGRO_BITMAP*nombre = load_image("img/score.png");
    ALLEGRO_BITMAP*acceuil=load_image("img/background.bmp");
    ALLEGRO_BITMAP*texte_acceuil=load_image("img/texteacceuil.png");
    ALLEGRO_BITMAP*new=load_image("img/new.png");
    ALLEGRO_BITMAP*end=load_image("img/pancarte.png");
    ALLEGRO_BITMAP*quitter=load_image("img/QUITTER.png");

    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_FONT* panneau = al_load_font("img/Minecraft.ttf", 24, 0);
    ALLEGRO_MOUSE_CURSOR*curseur = al_create_mouse_cursor(marteau,0,0);



    Taupe taupe[5] = { {30, 650, 0},
                       {520, 650, 0},
                       {850, 650, 0},
                       {1150, 650, 0},
                       {1600, 650, 0}
                    }; 

    al_start_timer(timer);
    
    bool redraw=true;
    
    int frame_visible=0;
    int random = 0;
    int score=0;
    int limit_random = getRandomInt(120,180);

    al_draw_bitmap(acceuil,0,0,0);
    al_draw_scaled_bitmap(texte_acceuil,0,0,800,160,500,100,900,200,0);
    al_draw_scaled_bitmap(new,0,0,800,167,650,400,600,125,0);
    al_draw_scaled_bitmap(quitter,0,0,550,164,775,580,340,101,0);

    for(int i=0; i<5; i++){
         al_draw_tinted_scaled_rotated_bitmap_region(taupe_img, 0, 97, 103, 100, al_map_rgb(255,255,255), 0, 0, taupe[i].position_x, 720, 2, 2, 0, 0);
    }
    //al_draw_bitmap(new,560,600,0);


    al_flip_display();

    //ALLEGRO_EVENT_QUEUE* event_queue= al_create_event_queue();
    //al_register_event_source(event_queue, al_get_keyboard_event_source());
    bool fin=false;
    while(!fin){
        al_wait_for_event(queue, &event);
        if(event.type== ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if(event.mouse.button == 1){
                int x = event.mouse.x;
                int y = event.mouse.y;
                    if (x >= 650 && x <= 1250  && y>= 400 && y <= 525){
                        fin=true;
            }
                    if(x >= 775 && x <= 1125  && y>= 580 && y <= 681){
                        al_destroy_display(fenetre);
                        al_destroy_event_queue(queue);}
            }
        }
        if(event.type== ALLEGRO_EVENT_KEY_DOWN && strcmp(al_keycode_to_name(event.keyboard.keycode),"ESCAPE")==0){
                    al_destroy_display(fenetre);
                    al_destroy_event_queue(queue);
        }

    }

 

    while(fin){

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
                            if (x >= taupe[i].position_x && x <= taupe[i].position_x+270  && y>= taupe[i].position_y && y <= taupe[i].position_y+250){
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

            double current_time = al_get_time();
            int remaining_time = 30 - (int)(current_time);

            if(random == limit_random){
                limit_random = getRandomInt(600,800);
                random = 0;
                random_taupe(taupe);
            }else{
                random++;
            }



            al_set_mouse_cursor(fenetre,curseur);

            al_draw_scaled_bitmap(fond,0,0,1920,1080,0,0,1920,1080,0);
            //al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 700, ALLEGRO_ALIGN_CENTER, "Mon score : %d", score);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 1200, 100, ALLEGRO_ALIGN_CENTER, "temps restant : %d", remaining_time);
            al_draw_scaled_bitmap(ecriture,0,0,800,172,0,0,800,172,0);
            al_draw_tinted_scaled_rotated_bitmap_region(nombre, 0+38*(score), 0, 38, 32, al_map_rgb(255,255,255), 0, 0, 800, 40, 3, 3, 0, 0);


            afficher_taupe(taupe,taupe_img,&frame_visible);          

            if (remaining_time <= 0) {
                al_draw_bitmap(fond, 0, 0, 0);
                al_draw_scaled_bitmap(end,0,0,2289,2154,400,-200,1100,1000,0);
                //al_draw_textf(panneau, al_map_rgb(255, 255, 255), 100, 500, ALLEGRO_ALIGN_CENTER, "baise ta mère %d",score);
                al_draw_scaled_bitmap(new,0,0,800,167,650,400,600,125,0);
                al_draw_scaled_bitmap(quitter,0,0,550,164,775,580,340,101,0);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 700, ALLEGRO_ALIGN_CENTER, "Bravo ton score est de: %d", score);
                al_wait_for_event(queue, &event);

                if(event.type== ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if(event.mouse.button == 1){
                        int x = event.mouse.x;
                        int y = event.mouse.y;
                        if (x >= 650 && x <= 1250  && y>= 400 && y <= 525){
                            fin=false;
                        }   
                        if(x >= 775 && x <= 1125  && y>= 580 && y <= 681){
                            al_destroy_display(fenetre);
                            al_destroy_event_queue(queue);}
            }
        }
                if(event.type== ALLEGRO_EVENT_KEY_DOWN && strcmp(al_keycode_to_name(event.keyboard.keycode),"ESCAPE")==0){
                    al_destroy_display(fenetre);
                    al_destroy_event_queue(queue);
        }

    }

            }
            al_flip_display();
        }

    al_destroy_bitmap(marteau);
    al_destroy_mouse_cursor(curseur);
    al_destroy_timer(timer);
    al_destroy_bitmap(fond);
    al_destroy_bitmap(taupe_img);
    al_destroy_bitmap(acceuil);
    al_destroy_bitmap(texte_acceuil);
    al_destroy_bitmap(nombre);
    al_destroy_bitmap(ecriture);
    al_destroy_font(font);
    al_destroy_font(panneau);

    }