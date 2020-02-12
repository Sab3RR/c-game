#include "background.h"

bool background::Init()
{
	sprite = createSprite("data/background.png");
	if (sprite == NULL)
	{
		return true;
	}
	getSpriteSize(sprite, w, h);
	getScreenSize(wscreen, hscreen);
	numw = ceil(wscreen / (float)w);
	numh = ceil(hscreen / (float)h);
	return false;
}

void background::draw()
{
	int i = 0;
	int j = 0;

	while (i < hscreen)
	{
		j = 0;
		while (j < wscreen)
		{
			drawSprite(sprite, j, i);
			j += w;
		}
		i += h;
	}
}
