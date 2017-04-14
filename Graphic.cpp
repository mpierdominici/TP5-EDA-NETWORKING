#include "Graphic.h"
#include <cstdlib>
#include <cstdio>


Graphic :: Graphic (uint32_t _nImages, char * imgFiles[])
{
	valid = false;	//estos valores se modificaran solo si la inicializacion es correcta
	display = NULL;
	images = NULL;
	nImages = 0;
	evQ = NULL;
	xMax = 0;
	yMax = 0;

   ALLEGRO_DISPLAY_MODE   disp_data;

	uint32_t i = 0;	//para indexar los punteros a bitmap y a string

	if (_nImages) { //verificar que se haya recibido al menos el fondo
		if ((images = (ALLEGRO_BITMAP **) calloc(_nImages, sizeof(ALLEGRO_BITMAP *))) != NULL) {
			if(al_init()) {
			//	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
			//	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
			//	if( (display = al_create_display(disp_data.width, disp_data.height)) != NULL ) {
				if( (display = al_create_display(800, 600)) != NULL ) {
					if(al_install_keyboard()) {
						if( (evQ = al_create_event_queue()) != NULL ) {
							if (al_init_image_addon()) {
								if ( (timer = al_create_timer(1.0/(double)FPS)) != NULL) {
									valid = true;	//ya se instalo todo, falta cargar los bitmaps

									for (i = 0; i<_nImages && imgFiles[i] != NULL && valid == true; i++) {
										if ( (images[i]=al_load_bitmap(imgFiles[i])) == NULL) {
											valid = false;
										}
									}

									if (valid == false) { //se cargo mal un bitmap, cerrar todo
										fprintf(stderr, "Unable to load bitmap (image id: %d)\n", i);
										i--;	//se incrementa uno mas de los que se inicializan correctamente

										while (i-- != 0) {
											al_destroy_bitmap(images[i]);
										}

										al_destroy_timer(timer);		//destruir todo lo que se inicializo
										timer = NULL;
										al_shutdown_image_addon();
										al_destroy_event_queue(evQ);
										evQ = NULL;	
										al_destroy_display(display);
										display = NULL;
										free(images);
										images = NULL;
									}
									else { //en este punto ya todo salio bien
										nImages = _nImages;
										//xMax = disp_data.width;
										//yMax = disp_data.height;
										xMax = 800; yMax = 600;

										al_register_event_source(evQ, al_get_keyboard_event_source());
									    al_register_event_source(evQ, al_get_timer_event_source(timer));

										al_clear_to_color(al_map_rgb(0,0,0));	//fondo negro
										al_draw_scaled_bitmap(images[0], 0, 0, al_get_bitmap_width(images[0]), 
														al_get_bitmap_height(images[0]),0, 0, xMax, yMax, 0);
										al_flip_display(); //mostrar el fondo

										al_start_timer(timer);
									}
								}
								else {
									fprintf(stderr, "Unable to create timer\n");
									al_shutdown_image_addon();		//si algo fallo, destruir todo lo que
									al_destroy_event_queue(evQ);	//ya se habia inicializado
									evQ = NULL;
									al_destroy_display(display);
									display = NULL;
									free(images);
									images = NULL;
								}
							}
							else {
								fprintf(stderr, "Unable to install image add-on\n");
								al_destroy_event_queue(evQ);
								evQ = NULL;
								al_destroy_display(display);
								display = NULL;
								free(images);
								images = NULL;
							}
						}
						else {
							fprintf(stderr, "Unable to create event queue\n");
							al_destroy_display(display);
							display = NULL;
							free(images);
							images = NULL;
						}
					}
					else {
						fprintf(stderr, "Unable to install keyboard\n");
						al_destroy_display(display);
						display = NULL;
						free(images);
						images = NULL;
					}
				}
				else {
					fprintf(stderr, "Unable to create display\n");
					free(images);
					images = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to initialize allegro\n");
				free(images);
				images = NULL;
			}
		}
	}
}


bool Graphic :: isValid()
{
	return valid;
}


Graphic :: ~Graphic()
{
	if (valid == false)
		return;

	while (nImages-- != 0) {
		al_destroy_bitmap(images[nImages]);
	}

	al_destroy_timer(timer);		//destruir todo lo que se inicializo
	timer = NULL;
	al_shutdown_image_addon();
	al_destroy_event_queue(evQ);
	evQ = NULL;	
	al_destroy_display(display);
	display = NULL;
	free(images);
	images = NULL;

	valid = false;
}


void Graphic :: drawBackground()
{
	al_clear_to_color(al_map_rgb(0,0,0));	//"borrar" lo que habia hasta el momento
	al_draw_scaled_bitmap(images[0], 0, 0, al_get_bitmap_width(images[0]), 
					al_get_bitmap_height(images[0]),0, 0, xMax, yMax, 0);
}


bool Graphic :: drawImage(uint32_t imgId, uint32_t x, uint32_t y, bool flipHorizontal)
{
	if (imgId >= nImages )
		return false; //parametros recibidos no validos

	al_draw_bitmap(images[imgId], x, y, flipHorizontal ? ALLEGRO_FLIP_HORIZONTAL : 0);
	// con ALLEGRO FLIP HORIZONTAL la imagen se pone al reves, con 0 tal como estaba
	return true;	//indicar que se pudo dibujar
}


void Graphic :: showChanges()
{
	al_flip_display();
}


//uint32_t Graphic :: getEvent()
//{	
//	ALLEGRO_EVENT ev;
//	uint32_t evCode = NO_EVENT;
//
//	// si al_get_next_event devuelve false, no se registro un nuevo evento
//	// en ese caso, queda el valor de evCode que ya se puso
//	if ( al_get_next_event(evQ, &ev) == true ) {
//		switch (ev.type) {
//
//			case ALLEGRO_EVENT_KEY_UP: { //se levanto una tecla. determinar cual
//				switch (ev.keyboard.keycode) {
//					case ALLEGRO_KEY_UP:		evCode = UP_UP;
//					break;
//					case ALLEGRO_KEY_LEFT:		evCode = LEFT_UP;
//					break;
//					case ALLEGRO_KEY_RIGHT:		evCode = RIGHT_UP;
//					break;
//					case ALLEGRO_KEY_W:			evCode = W_UP;
//					break;
//					case ALLEGRO_KEY_A:			evCode = A_UP;
//					break;
//					case ALLEGRO_KEY_D:			evCode = D_UP;
//					break;
//					case ALLEGRO_KEY_ESCAPE:	evCode = EXIT;
//					break;//el programa deberia terminar cuando la tecla se presiona, pero por las dudas 
//				}
//			}
//			break;
//
//			case ALLEGRO_EVENT_KEY_DOWN: { //se apreto una tecla. determinar cual
//				switch (ev.keyboard.keycode) {
//					case ALLEGRO_KEY_UP:		evCode = UP_DOWN;
//					break;
//					case ALLEGRO_KEY_LEFT:		evCode = LEFT_DOWN;
//					break;
//					case ALLEGRO_KEY_RIGHT:		evCode = RIGHT_DOWN;
//					break;
//					case ALLEGRO_KEY_W:			evCode = W_DOWN;
//					break;
//					case ALLEGRO_KEY_A:			evCode = A_DOWN;
//					break;
//					case ALLEGRO_KEY_D:			evCode = D_DOWN;
//					break;
//					case ALLEGRO_KEY_ESCAPE:	evCode = EXIT;
//					break;
//				}
//			}
//			break;
//
//			//el unico posible evento de timer es que paso el tiempo correspondiente a un frame
//			case ALLEGRO_EVENT_TIMER: 		evCode = TIMEOUT;
//			break;
//		}
//	}
//	
//	return evCode;
//}
