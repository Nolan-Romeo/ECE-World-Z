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

    ALLEGRO_BITMAP *barrel_img = load_image("img/barrel.bmp");
    ALLEGRO_BITMAP *explosion = load_image("img/explosion.bmp");
    ALLEGRO_BITMAP *background = load_image("img/background.bmp");
    ALLEGRO_BITMAP *chain = load_image("img/chain.bmp");
    ALLEGRO_BITMAP *liane = load_image("img/liane.bmp");
    ALLEGRO_BITMAP *perso = load_image("img/character.bmp");
    ALLEGRO_BITMAP *button = load_image("img/button1.bmp");
    ALLEGRO_BITMAP *button_pressed = load_image("img/button1_pressed.bmp");

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    bool redraw = true;

    int frame_bg = 0;

    double acc = 9.81*55;
    double vit = 0;
    double camera = 0;

    bool start = false;
    int intro = 0;

    Barrel barrel[2] = { [0 ... 1] = {48, false, false, 0, 0}};

    ALLEGRO_COLOR tinted = al_map_rgb(100,100,100);

    ALLEGRO_MOUSE_STATE mouse_pos;

    clock_t begin = clock();
    clock_t end;

    while (1){

        if(start){
            end = clock();
            unsigned long millis = (end - begin) * 1000 / CLOCKS_PER_SEC;
            barrel[0].chain_count = (barrel[0].pos-48)/32+1;
            barrel[1].chain_count = (barrel[1].pos-48)/32+1;
            vit = acc*(millis/1000.0);
            camera = (camera>=400)?camera :(vit*(millis/1000.0))/2;
            for(int i=0 ; i<2 ; i++){
                if(!barrel[i].stop && barrel[i].pos < 970) barrel[i].pos = (vit*(millis/1000.0))/2+48;
                else if(!barrel[i].stop){
                    barrel[i].explosion_state = true;
                }
            }
        }

        if(!al_is_event_queue_empty(queue)){
            al_get_next_event(queue, &event);
            switch (event.type){
                case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                    al_get_mouse_state(&mouse_pos);
                    break; 
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if((intro != 0 || intro!= 2 )&& mouse_pos.x >= 790 && mouse_pos.x <= 1150 && mouse_pos.y >= 486 && mouse_pos.y <= 594 ){
                        intro = 1;
                    }
                    break;    
                case ALLEGRO_EVENT_KEY_DOWN:
                    switch (event.keyboard.keycode){
                        case ALLEGRO_KEY_ESCAPE:
                            al_destroy_display(disp);
                            al_destroy_event_queue(queue);
                            return 0; 
                        case ALLEGRO_KEY_SPACE:
                            if( intro == 2  && start == false){
                                begin = clock();
                                start = true;
                            }
                            break;
                        case ALLEGRO_KEY_J:
                            if( (400-(frame_bg*10/3)+camera) >= 400) barrel[1].stop = true;
                            break;
                        case ALLEGRO_KEY_G:
                            if( (400-(frame_bg*10/3)+camera) >= 400) barrel[0].stop = true;   
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
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(255, 255, 255));

            if(intro == 1){
                frame_bg = (frame_bg==120)?120 : frame_bg+1;  
                tinted = al_map_rgb(255,255,255);
                if(frame_bg == 120 )intro = 2;   
            }

            int bg_y = 400-(frame_bg*10/3)+camera;
            al_draw_tinted_bitmap_region(background,tinted,0,bg_y,1920,1080,0,0,0);

            for(int j=0 ; j<2 ; j++){
                if(barrel[j].explosion_state){
                    if(barrel[j].frame_explosion<40){
                        barrel[j].frame_explosion++;
                    }else{
                        barrel[j].explosion_state = false;
                    }
                    al_draw_tinted_scaled_rotated_bitmap_region(explosion, 0+(barrel[j].frame_explosion/5)*64, 0, 64, 64, al_map_rgb(255,255,255), 0, 0, 786+160*j, 570, 3, 3, 0, 0);   
                }

                if( bg_y == 400 ){
                    for (int i = -1; i < barrel[j].chain_count+3; i++){
                        al_draw_bitmap(chain,860+160*j,(barrel[j].pos-32*i)-400,0);
                    }                    
                    al_draw_bitmap(barrel_img,816+160*j, (barrel[j].pos+64)-400,0);
                }
                else{
                    for (int i = -2; i < barrel[j].chain_count+3; i++){
                        al_draw_bitmap(chain,860+160*j,-400+(frame_bg*10/3)-32*i,0);
                    }                    
                    al_draw_bitmap(barrel_img,816+160*j,-400+(frame_bg*10/3)+96,0); 
                }

            }
            
            al_draw_bitmap(liane,830,-400+(frame_bg*10/3)-camera,0);
            al_draw_bitmap(liane,830+128+32,-400+(frame_bg*10/3)-camera,ALLEGRO_FLIP_HORIZONTAL);
            for (int i = 0; i < 2; i++)
            {
                al_draw_tinted_scaled_rotated_bitmap_region(perso,0,0,24,24,tinted,0, 0,800+160*i,735+(frame_bg*10/3)-camera,7,7,0,0);
            }
            
            if( intro == 0 ){
                if(mouse_pos.x >= 790 && mouse_pos.x <= 1150 && mouse_pos.y >= 486 && mouse_pos.y <= 594){
                    al_draw_tinted_scaled_rotated_bitmap_region(button_pressed,0,0,90,27,al_map_rgb(255,255,255),0,0,790,486,4,4,0,0);
                }else{
                    al_draw_tinted_scaled_rotated_bitmap_region(button,0,0,90,27,al_map_rgb(255,255,255),0,0,790,486,4,4,0,0);
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