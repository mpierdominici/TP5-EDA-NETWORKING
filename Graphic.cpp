#include "Graphic.h"
#include <cstdlib>
#include <cstdio>


#define DEBUG


Graphic :: Graphic (uint32_t _nImages, char * imgFiles[])
{
	valid = false;	//estos valores se modificaran solo si la inicializacion es correcta
	display = NULL;
	images = NULL;
	nImages = 0;
	//evQ = NULL;
	xMax = 0;
	yMax = 0;

	uint32_t i = 0;	//para indexar los punteros a bitmap y a string

	if (_nImages) { //verificar que se haya recibido al menos el fondo
		if ((images = (ALLEGRO_BITMAP **) calloc(_nImages, sizeof(ALLEGRO_BITMAP *))) != NULL) {
			if(al_init()) {
				if (al_init_image_addon()) {
					valid = true;	//ya se instalo todo, falta cargar los bitmaps

					for (i = 0; i<_nImages && imgFiles[i] != NULL && valid == true; i++) {
						if ( (images[i]=al_load_bitmap(imgFiles[i])) == NULL) {
							valid = false;
						}
					}

					if (valid == false) { //se cargo mal un bitmap, cerrar todo
						fprintf(stderr, "Unable to load bitmap (image id: %d)\n", i);
						i--;	//se incrementa uno mas de los que se inicializan correctamente

						while (i-- != 1) {
							al_destroy_bitmap(images[i]);
						}

						al_shutdown_image_addon();
						free(images);
						images = NULL;
					}
					else { //en este punto ya todo salio bien
						nImages = _nImages;
					}
				}
				else {
					fprintf(stderr, "Unable to install image add-on\n");
					al_destroy_display(display);
					display = NULL;
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

	al_shutdown_image_addon();			//destruir todo lo que se inicializo
	al_destroy_display(display);
	display = NULL;
	free(images);
	images = NULL;

	valid = false;
}

bool Graphic :: setupDisplay()
{
	if (valid == false)
		return false;

#ifdef DEBUG
	if( (display = al_create_display(800, 600)) != NULL ) {
		xMax = 800;
		yMax = 600;
#else
	ALLEGRO_DISPLAY_MODE   disp_data;

	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	if( (display = al_create_display(disp_data.width, disp_data.height)) != NULL ) {
		xMax = disp_data.width;
		yMax = disp_data.height;
#endif // DEBUG							
		al_clear_to_color(al_map_rgb(0,0,0));	//fondo negro
		al_flip_display();
		return true;	
	}
	else {
		valid = false;	//hubo error!
		return false;
	}
}


void Graphic:: destroyDisplay()
{
	al_destroy_display(display);
	display = NULL;
}


void Graphic :: drawBackground()
{
	al_clear_to_color(al_map_rgb(0,0,0));	//"borrar" lo que habia hasta el momento
}


bool Graphic :: drawImage(uint32_t imgId, float x, float y, bool flipHorizontal)
{
	if (imgId >= nImages)
		return false; //parametros recibidos no validos

	al_draw_bitmap(images[imgId], x, y, flipHorizontal ? ALLEGRO_FLIP_HORIZONTAL : 0);
	// con ALLEGRO FLIP HORIZONTAL la imagen se pone al reves, con 0 tal como estaba
	return true;	//indicar que se pudo dibujar
}


void Graphic :: showChanges()
{
	al_flip_display();
}
