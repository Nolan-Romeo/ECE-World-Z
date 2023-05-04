#include "std_iso.h"

void solve_euler(double *theta, double *omega, double dt, double m, double g, double l, double k) {
	*theta = *theta + (*omega) * dt;
	*omega = *omega - (g/l)*sin(*theta)*dt - (k/m)*(*omega)*dt;
	printf("theta(0) = %.3f, omega(0) = %.3f\n", *theta, *omega);
}

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

    int cameraX, cameraY;
    double zoom = 45.0;

    double theta = 0.0; // en rad
    double omega = 0.0; // en rad/s

    double g = 9.81;  // constante de gravité
	double l = 10.0;   // longueur du pendule
	double m = 10.0;   // masse du pendule
	double k = 1.0;   // constante d'amortissement

    double p_x, p_y, v_x, v_y;

	int force = 0; // force excitatrice

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
                            break;
						case ALLEGRO_KEY_Q:
                            force = -1;
							break;
						case ALLEGRO_KEY_D:
							force = 1;
							break;
                        default:
                            break;
                    } 
                    break;
				case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode){
					case ALLEGRO_KEY_Q:
						force = 0;
						break;
					case ALLEGRO_KEY_D:
						force = 0;
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

            if(!launched){ // mouvement d'oscillation sur le pendule

                if(force == 1){
                    omega += cos(theta)*0.015;
                }else if(force == -1){
                    omega -= cos(theta)*0.015;
                }

				p_x = sin(theta)*l;
            	p_y = cos(theta)*l;

                double v = omega*l;
                v_x = cos(theta)*v;
                v_y = -sin(theta)*v;

				player_angle = -theta;

            }else if(p_y < 12){

				v_y += 9.81/fps;

				p_x += v_x/fps;
				p_y += v_y/fps;

				player_angle = 0;
            }

            solve_euler(&theta, &omega, 1.0/fps, m, g, l/15, k);

            cameraX = -(p_x*zoom)+1920/2;
            cameraY = -(p_y*zoom)+1080/2;

            al_draw_bitmap(background, (cameraX%3510)-500, cameraY-300, 0);
            al_draw_bitmap(background, (cameraX%3510)-500+3510, cameraY-300, 0);
            al_draw_bitmap(background, (cameraX%3510)-500-3510, cameraY-300, 0);
            al_draw_filled_rectangle(0, 0, 1920, cameraY-300, al_map_rgb(30,32,30));
            al_draw_filled_rectangle(0, cameraY-300+1080, 1920, 1080, al_map_rgb(30,32,30));

			al_draw_scaled_rotated_bitmap(liane, 22, 0, cameraX, cameraY, 2.5, 2.5, -theta, 0);
			al_draw_tinted_scaled_rotated_bitmap_region(character, 48, 0, 24, 24, al_map_rgb(255, 255, 255), 12, 12, 1920/2, 1080/2, 4, 4, player_angle, 0);
			al_draw_line(1920/2, 1080/2, 1920/2+v_x*5, 1080/2+v_y*5, al_map_rgb(255, 255, 255), 4);

			al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "angle: %f", theta);
            al_draw_textf(font, al_map_rgb(255,255,255), 10, 30, 0, "vitesse angulaire: %f", omega);
            al_draw_textf(font, al_map_rgb(255,255,255), 10, 50, 0, "force: %d", force);
			al_draw_textf(font, al_map_rgb(255,255,255), 10, 70, 0, "posX: %f, posY: %f", p_x, p_y);

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