#ifndef ANIMATION_H
#define ANIMATION_H


#include "Graphic.h"
#include <cstdint>

class Animation
{
private:
	static Graphic * g;
	bool isValid;
	bool moves;
	bool goLeft;
	uint32_t nImages;
	uint32_t img0Id;
	double frameRate;
	uint32_t moveInFrame;
	uint32_t speed;

public:
	Animation() {; }
	bool setup(uint32_t nImages, uint32_t img0Id, double frameRate, uint32_t moveInFrame, uint32_t speed, bool goLeft, Graphic * g = NULL);
	void move(uint32_t y, uint32_t x = 0);
};




#endif //ANIMATION_H