#ifndef GRAPHIC_H
#define GRAPHIC_H

/*	

	Instituto Tecnológico de Buenos Aires
	22.08 - Algoritmos y estructura de datos

	Trabajo práctico n° 6: Networking

	Autores:	Máspero, Martina		- legajo 
				Parra, Rocío			- legajo 57.669
				Pierdominici, Matías	- legajo

	Fecha de entrega: martes 18 de abril de 2017

*********************************************************************************

	Esta clase controla ...
*/



extern "C" {
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
}
#include <cstdint>


class Graphic {
private:
	bool valid;
	bool background;
	ALLEGRO_DISPLAY * display;
	uint32_t xMax;
	uint32_t yMax;
	ALLEGRO_BITMAP ** images;
	uint32_t nImages;
	//ALLEGRO_EVENT_QUEUE * evQ;
	//ALLEGRO_TIMER * timer;

public:
	Graphic (uint32_t _nImages, char * imgFiles[]);
	// recibe el numero de imagenes que debe cargar, y un arreglo de punteros a los paths de los
	//archivos donde se encuentra cada una de ellas.

	~Graphic();

	bool drawImage(uint32_t imgId, float x, float y, bool flipHorizontal);
	// dibuja la imagen que indica imgId. la esquina SUPERIOR IZQUIERDA va a estar en x,y
	// si flipHorizontal es true, dibuja la imagen "mirando para el otro lado", si no la deja
	//tal como esta
	// devuelve true si los parametros recibidos eran validos y viceversa

	uint32_t getXMax() {return xMax;}
	uint32_t getYMax() {return yMax;}	

	void drawBackground();
	
	void showChanges();

	bool isValid();
};



#endif //GRAPHIC_H

