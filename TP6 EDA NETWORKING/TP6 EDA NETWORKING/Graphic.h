#ifndef GRAPHIC_H
#define GRAPHIC_H

/*	

	Instituto Tecnol�gico de Buenos Aires
	22.08 - Algoritmos y estructura de datos

	Trabajo pr�ctico n� 6: Networking

	Autores:	M�spero, Martina		- legajo 57.120
				Parra, Roc�o			- legajo 57.669
				Pierdominici, Mat�as	- legajo 58.498

	Fecha de entrega: martes 18 de abril de 2017


*/



extern "C" {
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
}
#include <cstdint>


class Graphic {
private:
	bool valid;
	ALLEGRO_DISPLAY * display;
	uint32_t xMax;
	uint32_t yMax;
	ALLEGRO_BITMAP ** images;
	uint32_t nImages;

public:
	Graphic (uint32_t _nImages, char * imgFiles[]);
	// recibe el numero de imagenes que debe cargar, y un arreglo de punteros a los paths de los
	//archivos donde se encuentra cada una de ellas.

	~Graphic();

	bool setupDisplay();
	void destroyDisplay();

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

