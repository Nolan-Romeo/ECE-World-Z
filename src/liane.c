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
	ALLEGRO_BITMAP *liane = load_image("img/liane.bmp");

    bool redraw = true;

    bool launched = false;

    double cameraX, cameraY;

    double x01 = 0.0, y01 = 0.0, x11 = 0.0, y11 = 0.0, x21 = 0.0, y21 = 0.0;
    double v01 = 0.0, vx01 = 0.0, vy01 = 0.0;
    double theta = 0.0; // en rad
    double v_theta = 0.0; // en rad/s
    double t = 0.0, t2 = 0.0; // en s

    double zoom = 45.0;

    const double g = 9.81;
    const double l = 10.0;
    const double periode = 2*M_PI*sqrt(l/g); // en s
    const double w0 = 2*M_PI/periode; // en s^-1
	
	double lambda = 12;
	double m = 50; // en kg
	double alpha = lambda/(2*m);
	double w = sqrt(pow(w0, 2)-pow(alpha, 2));
	double A = -(3*M_PI)/4;
	double B = (alpha*A)/w;

	double F_A = 0.0; // force excitatrice
	int force_direction = 0;
	int t_force = 0;

	double player_angle = 0;

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
						case ALLEGRO_KEY_Q:
							F_A = 0;
							force_direction = -1;
							break;
						case ALLEGRO_KEY_D:
							F_A = 0;
							force_direction = 1;
							break;
                        default:
                            break;
                    } 
                    break;
				case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode){
					case ALLEGRO_KEY_Q:
						force_direction = 0;
						break;
					case ALLEGRO_KEY_D:
						force_direction = 0;
						break;
					default:
						break;
					}
				
                default:
                    break;
            }
        }

        if(redraw && al_is_event_queue_empty(queue)){

            al_clear_to_color(al_map_rgb(0, 0, 0));

            t += 1.0/60.0;

            //theta = -3*M_PI/4*cos(w0*t);

			theta = exp(-alpha*t)*(A*cos(w*t) + B*sin(w*t));

            x01 = sin(theta)*l;
            y01 = cos(theta)*l;

            if(!launched){ // mouvement d'oscillation sur le pendule
                x11 = x01; y11 = y01;

				double Q = w0/(2*alpha);

				v_theta = -alpha*exp(-alpha*t)*(A*cos(w*t) + B*sin(w*t)) + exp(-alpha*t)*(-A*w*sin(w*t) + B*w*cos(w*t));

                //v_theta = w0*3*M_PI/4*sin(w0*t);

                v01 = v_theta*l;
                vx01 = v01*cos(theta);
                vy01 = v01*sin(theta);

                cameraX = -(x01*zoom)+1920/2;
                cameraY = -(y01*zoom)+1080/2;

				al_draw_bitmap(background, cameraX-500, cameraY-300, 0);
				al_draw_bitmap(background, cameraX-500-3510, cameraY-300, 0);
				al_draw_bitmap(background, cameraX-500+3510, cameraY-300, 0);
				al_draw_filled_rectangle(0, 0, 1920, cameraY-300, al_map_rgb(30,32,30));
				al_draw_filled_rectangle(0, cameraY-300+1080, 1920, 1080, al_map_rgb(30,32,30));

				player_angle = -theta;

            }else {
                t2 += 1.0/60.0;

				if(0.5*g*pow(t2,2)-vy01*t2+y11 < 12){
					x21 = vx01*t2+x11;
                	y21 = 0.5*g*pow(t2,2)-vy01*t2+y11;
				}

                cameraX = -(x21*zoom)+1920/2;
                cameraY = -(y21*zoom)+1080/2;

				al_draw_bitmap(background, cameraX-500, cameraY-300, 0);
				al_draw_bitmap(background, cameraX-500-3510, cameraY-300, 0);
				al_draw_bitmap(background, cameraX-500+3510, cameraY-300, 0);
				al_draw_filled_rectangle(0, 0, 1920, cameraY-300, al_map_rgb(30,32,30));
				al_draw_filled_rectangle(0, cameraY-300+1080, 1920, 1080, al_map_rgb(30,32,30));

				player_angle = 0;
            }

            //al_draw_line(cameraX, cameraY, x01*zoom+cameraX, y01*zoom+cameraY, al_map_rgb(255, 255, 255), 2);
			al_draw_scaled_rotated_bitmap(liane, 22, 0, cameraX, cameraY, 2.5, 2.5, -theta, 0);
			al_draw_tinted_scaled_rotated_bitmap_region(character, 48, 0, 24, 24, al_map_rgb(255, 255, 255), 12, 12, 1920/2, 1080/2, 4, 4, player_angle, 0);
            /* al_draw_line(x01*zoom+cameraX, y01*zoom+cameraY, x01*zoom+vx01*5+cameraX, y01*zoom+cameraY, al_map_rgb(255, 0, 0), 2);
            al_draw_line(x01*zoom+cameraX, y01*zoom+cameraY, x01*zoom+cameraX, y01*zoom-vy01*5+cameraY, al_map_rgb(255, 0, 0), 2);
            al_draw_line(x01*zoom+cameraX, y01*zoom+cameraY, x01*zoom+vx01*5+cameraX, y01*zoom-vy01*5+cameraY, al_map_rgb(0, 255, 0), 2); */

            al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "sense poussée : %d", force_direction);
			al_draw_textf(font, al_map_rgb(255,255,255), 10, 20, 0, "vitesse : %0.1f", v_theta);
			al_draw_textf(font, al_map_rgb(255,255,255), 10, 30, 0, "F_A : %0.01f", F_A);

            al_flip_display();

            redraw = false;
        }
    }

	al_destroy_bitmap(background);
	al_destroy_bitmap(character);
	al_destroy_bitmap(liane);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}