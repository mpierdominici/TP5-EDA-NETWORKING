#ifndef ANIMATION_H
#define ANIMATION_H


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

#include "Graphic.h"
#include <cstdint>

class Animation
{
private:
	static Graphic * g;
	//controlador de la parte grafica (uno solo por programa).
	//debe tener cargadas todas las imagenes de todas las animaciones

	bool isValid;	//true si setup se hizo correctamente y viceversa. no se puede llamar a play si no es true!

	uint32_t nImages;	//cantidad de imagenes que componen la animacion
	uint32_t img0Id;	//id de la primera imagen, tal como se inicializo en Graphic

	uint32_t width;		//ancho y altura de cada imagen (si hay tamanos distintos, elegir el mas grande)
	uint32_t height;

	double frameRate;		//cada cuantos segundos debe actualizarse el frame
	uint32_t moveInFrame;	//cada cuantos frames se debe modificar la posicion de la animacion
	int32_t speed;			//cuantos pixeles se mueve la animacion por paso. POSITIVO HACIA LA DERECHA. 0 si debe quedarse quieta

public:
	Animation() { ; }
	bool setup(uint32_t nImages, uint32_t img0Id, uint32_t width, uint32_t height,
		double frameRate, uint32_t moveInFrame, int32_t speed, Graphic * g = NULL);
	//	guarda todos los datos correspondientes a la animacion.
	//	si la animacion debe quedarse quieta, moveInFrame y speed deben ser 0 y goLeft se ignorara.
	//	graphic solo se guardara la primera vez que se llame a esta funcion para algun objeto Animation
	//con un parametro distinto de NULL

	void play();
	//	reproduce la totalidad de la animacion
	//	para las que se mueven, lo hace de un extremo al otro de la pantalla en el sentido que corresponda
	//	para las que se quedan quietas, hasta que se queda sin imagenes.
	//	en ambos casos la coordenada en Y inicial sera en la mitad de la pantalla, y para las que se quedan
	//quietas lo mismo con X.
};




#endif //ANIMATION_H