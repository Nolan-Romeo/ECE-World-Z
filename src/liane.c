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

    bool redraw = true;
    int time = 0;
    int periode = 120;
    double w0 = (M_PI*2)/periode;
    int longueur = 500;
    int cx = 1920/3, cy = 1080/4;
    double v_max = w0*M_PI_2;
    int ball_radius = 20;
    double resolution_trainee = 2; // plus la valeur est élevé moins des cercles sont dessinés
    int longeur_trainee = 200; // plus la valeur est élevée plus la trainée est courte

    double angle = -M_PI_2;
    double speed = 0;
    double gravity = 0.000002;

    double angle0 = angle;
    double speed0 = speed;
    int x0 = 0, y0 = 0;

    bool launched = false;

    while (1){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

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
                        break;
                    default:
                        break;
                } 
                break;
            default:
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(0, 0, 0));

            time = (time+1)%periode;
            angle = M_PI_2*cos(time*w0);
            speed = w0*M_PI_2*sin(time*w0);

            int ball_x = cx+sin(angle)*longueur;
            int ball_y = cy+cos(angle)*longueur;

            if(!launched){
                angle0 = angle;
                speed0 = speed;
                x0 = ball_x;
                y0 = ball_y;
            }

            int w = speed*longueur;

            al_draw_line(cx, cy, ball_x, ball_y, al_map_rgb(255,255,255), 4);

            for(int i=0; i<ball_radius/resolution_trainee; i++){
                al_draw_filled_circle(cx+sin(angle+M_PI/longeur_trainee*i*(speed/v_max)*resolution_trainee)*longueur, cy+cos(angle+M_PI/longeur_trainee*i*(speed/v_max)*resolution_trainee)*longueur, ball_radius-i*resolution_trainee, al_map_rgb(255,255,255));
            }  

            if(speed0 < 0){
                for(double x=0.0; x<1600.0; x+=10){
                    al_draw_filled_circle(x0+x, y0+(gravity*x*x)/(2*speed0*speed0*cos(angle0)*cos(angle0))-tan(angle0)*x, 4, al_map_rgb(255,255,255));
                }
            }else{
                for(double x=0.0; x>-1600.0; x-=10){
                    al_draw_filled_circle(x0+x, y0+(gravity*x*x)/(2*speed0*speed0*cos(angle0)*cos(angle0))-tan(angle0)*x, 4, al_map_rgb(255,255,255));
                }
            }

            printf("v max : %f \n", speed);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}