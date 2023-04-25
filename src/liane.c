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

    bool redraw = true;

    double angle = 0;
    double speed = 0;
    double gravity = 0.05;

    double time = 0;
    int t_launch = 0;
    int meter_to_pixel = 20;
    double periode = 2; // en secondes
    double max_angle = M_PI/4*3;
    double w0 = (M_PI*2)/periode;
    int longueur = 10*meter_to_pixel;
    int cx = 1920/3, cy = 1080/4;

    int ball_x = cx+sin(angle)*longueur;
    int ball_y = cy+cos(angle)*longueur;

    int ballisticX, ballisticY;

    double cameraX = 1920/2-ball_x, cameraY = 1080/2-ball_y;

    double v_max = w0*max_angle;
    int ball_radius = 1*meter_to_pixel;
    double resolution_trainee = 2; // plus la valeur est élevé moins des cercles sont dessinés
    int longeur_trainee = 100*meter_to_pixel; // plus la valeur est élevée plus la trainée est courte

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
                        int t = 0;
                        int t0 = 0;
                        t_launch = t0;
                        while(y0+(0.5*gravity*t*t-speed0*sin(angle0)*t) < cy+longueur+1*meter_to_pixel){
                            t+=1;
                        }
                        int distanceX = x0-cx+speed0*cos(angle0)*t;
                        printf("distance parcourue : %d", distanceX/meter_to_pixel);
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

            time = (time < periode)? time+1.0/fps: 0;
            angle = max_angle*cos(time*w0+M_PI_2); // en radians
            speed = -w0*max_angle*sin(time*w0+M_PI_2); // en radians par seconde

            ball_x = cx+sin(angle)*longueur;
            ball_y = cy+cos(angle)*longueur;

            if(!launched){
                angle0 = angle;
                speed0 = speed;
                x0 = ball_x;
                y0 = ball_y;
            }

            double w = speed*longueur; // en pixels par seconde

            ballisticX = x0+speed0*cos(angle0)*t_launch/(10.0/meter_to_pixel);
            ballisticY = y0+(0.5*gravity*t_launch*t_launch-speed0*sin(angle0)*t_launch)/(10.0/meter_to_pixel);

            cameraX = 1920/2-ballisticX;
            cameraY = 1080/2-ballisticY;

            al_draw_line(cx+cameraX, cy+cameraY, ball_x+cameraX, ball_y+cameraY, al_map_rgb(255,255,255), 4);

            for(int i=0; i<ball_radius/resolution_trainee; i++){
                al_draw_filled_circle(cx+sin(angle-M_PI/longeur_trainee*i*(speed/v_max)*resolution_trainee)*longueur+cameraX, cy+cos(angle-M_PI/longeur_trainee*i*(speed/v_max)*resolution_trainee)*longueur+cameraY, ball_radius-i*resolution_trainee, al_map_rgb(255,255,255));
            }  

            if(y0+(0.5*gravity*t_launch*t_launch-speed0*sin(angle0)*t_launch)/(10.0/meter_to_pixel) < cy+longueur+1*meter_to_pixel && launched){
                al_draw_filled_circle(x0+speed0*cos(angle0)*t_launch/(10.0/meter_to_pixel)+cameraX, y0+(0.5*gravity*t_launch*t_launch-speed0*sin(angle0)*t_launch)/(10.0/meter_to_pixel)+cameraY, 4, al_map_rgb(255,255,255));
                t_launch+=1;
            }

            al_draw_line(cx+cameraX, cy+longueur+1*meter_to_pixel+cameraY, cx+10000+cameraX, cy+longueur+1*meter_to_pixel+cameraY, al_map_rgb(255,255,255), 1);
            for(int i = 0; i < 10000; i += meter_to_pixel*10){
                al_draw_line(cx+i+cameraX, cy+longueur+1*meter_to_pixel+cameraY, cx+i+cameraX, cy+longueur+1*meter_to_pixel+10+cameraY, al_map_rgb(255,255,255), 1);
            }

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "vitesse : %f", w);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}