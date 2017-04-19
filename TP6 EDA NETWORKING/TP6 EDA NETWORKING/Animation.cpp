#include "Animation.h"


Graphic * Animation :: g;


bool Animation :: setup(uint32_t nImages, uint32_t img0Id, uint32_t width, uint32_t height,
						double frameRate, uint32_t moveInFrame, int32_t speed, Graphic * g)
{
	isValid = false;

	if ( this->g == NULL && g != NULL && g->isValid() )
		this->g = g;

	if ( nImages!=0 && frameRate!=0  && width!= 0  && height!=0 && !(bool(moveInFrame) ^ bool(speed)) ) {
		//debe haber al menos 1 imagen, el frameRate y el ancho y alto de las imagenes no pueden ser 0,
		//y la velocidad a la que se mueve la animacion solo puede ser 0 si la misma debe moverse cada 0 frames y viceversa
		isValid = true;
		this->frameRate = frameRate;
		this->nImages = nImages;
		this->img0Id = img0Id;
		this->width = width;
		this->height = height;
		this->moveInFrame = moveInFrame;
		this->speed = speed;
	}

	return isValid;
}



void Animation :: play()
{
	if ( !isValid || g == NULL || !g->isValid() )
		return; //no hacer nada si no fue valida la inicializacion de la parte grafica o de la animacion

	float xMax;					//para las animaciones que se mueven, coordenada final en x
	bool reachedEnd = false;	//indicador de si termino o no de reproducirse
	uint32_t frameCounter = 0;	//indicador de que frame tiene que mostrarse

	float y = (g->getYMax() - float(height))/2.0;	//setear la altura en el medio del display
	float x;

	if (speed < 0) {
		x = g->getXMax();		//si la velocidad es negativa, se mueve desde la derecha a la izquierda
		xMax = -(float)width;
	}
	else if (speed > 0) {
		x = -(float)width;		//si la velocidad es positiva, se mueve de izquierda a derecha
		xMax = g->getXMax();
	}
	else {
		x = (g->getXMax() - float(width))/2.0;	//si la velocidad es 0, setear x en el medio del display
	}

	while (reachedEnd == false) { //reproducir la totalidad de la animacion
		for (frameCounter = 0; frameCounter < nImages; frameCounter++) { //reproducir un ciclo de imagenes
			g->drawBackground();
			g->drawImage(img0Id + frameCounter, x, y, false);	//mostrar la imagen en la coord que esta ahora
			g->showChanges();
			al_rest(frameRate);									//permitir que el usuario vaya viendo como se mueve
			
			if (speed != 0) {  
				if ((frameCounter % moveInFrame) == 0 ) { //mover la animacion cuando corresponda
					if ((speed>0 && x>xMax) || (speed<0 && x<xMax) )
							reachedEnd = true;	//indicar que se termino el display
						else
							x+=speed;
				}
			}
		}
	
		if(speed == 0)
		reachedEnd = true;	//para las que no se mueven, el loop se ejecuta una sola vez
	}
}