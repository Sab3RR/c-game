#pragma once
#include <iostream>
#include <Windows.h>
#include <Framework.h>
#include <cmath>
#include <vector>
#include "struct.h"

class background
{
public:
	bool Init();
	void draw();

private:
	Sprite* sprite;
	int w;
	int h;
	int wscreen;
	int hscreen;
	int numw;
	int numh;
};

