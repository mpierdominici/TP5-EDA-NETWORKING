#ifndef GRAPHIC_H
#define GRAPHIC_H

/*	

	Instituto Tecnológico de Buenos Aires
	22.08 - Algoritmos y estructura de datos

	Trabajo práctico n° 4: Worms

	Autores:	Inhargue, Ignacio	- legajo 57.681
				Navarro, Pablo		- legajo 57.775
				Parra, Rocío		- legajo 57.669

	Fecha de entrega: martes 4 de abril de 2017

*********************************************************************************

	Esta clase controla los eventos generados poor el input y el timer del juego worms, 
así como la representación gráfica del mismo.

*/



extern "C" {
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
}
#include <cstdint>


#define FPS	50	

enum eventCodes { NO_EVENT, TIMEOUT, EXIT,  
// NO EVENT: no se registro nada desde el ultimo evento
// TIMEOUT: paso el tiempo que hay que esperar entre frame y frame
// EXIT: terminar el juego
	W_UP, W_DOWN, A_UP, A_DOWN, D_UP, D_DOWN,
	UP_UP, UP_DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN };
//codigos de evento de teclado: la primer palabra indica a tecla corresponde y la segunda si se apreto o solto
//p.ej: W_UP == se solto la tecla W
//		LEFT_DOWN == se apreto la flecha de la izquierda



class Graphic {
private:
	bool valid;
	ALLEGRO_DISPLAY * display;
	uint32_t xMax;
	uint32_t yMax;
	ALLEGRO_BITMAP ** images;
	uint32_t nImages;
	ALLEGRO_EVENT_QUEUE * evQ;
	ALLEGRO_TIMER * timer;

public:
	Graphic (uint32_t _nImages, char * imgFiles[]);
	// recibe el numero de imagenes que debe cargar, y un arreglo de punteros a los paths de los
	//archivos donde se encuentra cada una de ellas.
	// la primera imagen va a ser utilizada como fondo, con lo cual _nImages DEBE SER AL MENOS 1. 
	// el resto (si las hubiera) se van a poder dibujar con drawImage(). el id de cada imagen sera
	//igual a indice dentro de imgFiles[] que tenia el path.
	//	p.ej: si quiero dibujar la imagen cuyo path esta guardado en imgFiles[4], el id que le voy
	//a tener que pasar a drawImage() es 4. si le paso un numero mayor o igual a nImages, va a ser
	//error

	~Graphic();

	bool drawImage(uint32_t imgId, uint32_t x, uint32_t y, bool flipHorizontal);
	// dibuja la imagen que indica imgId. la esquina SUPERIOR IZQUIERDA va a estar en x,y
	// si flipHorizontal es true, dibuja la imagen "mirando para el otro lado", si no la deja
	//tal como esta
	// devuelve true si los parametros recibidos eran validos y viceversa
	// notese que en cuanto a x e y, solo verifica que no se excedan del display, no si 
	//una parte de la imagen queda afuera a pesar de que su coordenada inicial sea correcta

	uint32_t getXMax() {return xMax;}
	uint32_t getYMax() {return yMax;}	

	void drawBackground();
	
	void showChanges();

	bool isValid();

//	uint32_t getEvent(); //los eventos se devuelven como estan indicados en eventCodes (arriba)
};



#endif //GRAPHIC_H

