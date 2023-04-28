#include "std_iso.h"

int main(int argc, char* argv[]){

    srand(time(NULL));
    assert(al_init());
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    double fps = 60.0;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / fps);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    ALLEGRO_BITMAP *background = load_image("img/background_liane.bmp");
    ALLEGRO_BITMAP *character = load_image("img/character.bmp");

    bool redraw = true;

    bool launched = false;

    double x = 0.0, y = 0.0, x2 = 0.0, y2 = 0.0;
    double v, vx, vy;
    double theta = 0.0; // en rad
    double v_theta = 0.0; // en rad/s
    double t = 0.0, t2 = 0.0; // en s

    double zoom = 30.0;

    const double g = 9.81;
    const double l = 10.0;
    const double periode = 2*M_PI*sqrt(l/g); // en s
    const double w0 = 2*M_PI/periode; // en s^-1

    while (1){

        ALLEGRO_EVENT event;

        if(!al_event_queue_is_empty(queue)){
            al_get_next_event(queue, &event);

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
                        case ALLEGRO_KEY_SPACE:
                            launched = !launched;
                            t2 = 0.0;
                            break;
                        default:
                            break;
                    } 
                    break;
                default:
                    break;
            }
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(0, 0, 0));

            if(!launched){ // mouvement d'oscillation sur le pendule
                t += 1.0/60.0;

                theta =-3*M_PI/4*cos(w0*t);

                v_theta = -w0*3*M_PI/4*sin(w0*t);

                v = v_theta*l;
                vx = v*cos(theta*t);
                vy = v*sin(theta*t);

                x = sin(theta)*l;
                y = cos(theta)*l;                

                al_draw_line(500, 500, 500+x*zoom, 500+y*zoom, al_map_rgb(255, 255, 255), 2);
            }else {

                t2 += 1.0/60.0;

                x2 = vx*t2+x;
                y2 = -0.5*g*pow(t2,2)+vy*t2+y;

                al_draw_filled_circle(500+x2*zoom, 500+y2*zoom, 4, al_map_rgb(255, 255, 255));
            }

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "vitesse : %0.1f", v_theta);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}