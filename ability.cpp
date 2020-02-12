#include "ability.h"

void ability::draw(int xspos, int yspos)
{
	drawSprite(sprite, coords.x - size.x / 2 - xspos, coords.y - size.y / 2 - yspos);
}