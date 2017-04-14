#include "Animation.h"


Graphic * Animation :: g;


bool Animation :: setup(uint32_t nImages, uint32_t img0Id, double frameRate, uint32_t moveInFrame, uint32_t speed, bool goLeft, Graphic * g)

{
	isValid = false;

	if ( this->g == NULL && g != NULL && g->isValid() )
		this->g = g;

	if ( nImages!=0 && frameRate!=0  && !(bool(moveInFrame) ^ bool(speed)) ) {
		//debe haber al menos 1 imagen, frameRate no puede ser 0,
		//y la velocidad a la que se mueve la animacion solo puede ser 0 si la misma debe moverse cada 0 frames y viceversa
		isValid = true;
		this->frameRate = frameRate;
		this->nImages = nImages;
		this->img0Id = img0Id;
		this->moveInFrame = moveInFrame;
		this->speed = speed;
		this->goLeft = goLeft;

		if (speed == 0)
			moves = false;
		else
			moves = true;
	}

	return isValid;
}



void Animation :: move( uint32_t y, uint32_t x )
{
	if ( !isValid || g == NULL || !g->isValid() )
		return;

	uint32_t i = 0;

	if (moves) {
		uint32_t xMax = 0;
		bool reachedEnd = false;
		uint32_t frameCounter = 0;

		if (goLeft == true) {
			x = g->getXMax();
		}
		else {
			x = 0;
			xMax = g->getXMax();
		}

		while (reachedEnd == false) {
			for (i = 0; i <nImages && reachedEnd == false; i++) {
				g->drawBackground();
				g->drawImage(img0Id + i, x, y, false);
				g->showChanges();

				if ( (frameCounter % moveInFrame) == 0 ) {
					if (goLeft) {
						if (x<speed)
							reachedEnd = true;
						else
							x-=speed;
					}
					else {
						if (x>xMax)
							reachedEnd = true;
						else
							x+=speed;
					}
				}

				frameCounter++;
				al_rest(frameRate);
			}
		}
	}
	else {
		for (i = 0; i <nImages; i++) {
				g->drawBackground();
				g->drawImage(img0Id + i, x, y, false);
				g->showChanges();
				al_rest(frameRate);
		}
	}
}