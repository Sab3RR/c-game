#pragma once
#include <Windows.h>
#include <Framework.h>
#include <utility>
#include <stdint.h>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <list>
#include <ctime>
#include "struct.h"

class ability
{
public:
	float2 coords;
	int2 size;
	Sprite* sprite;
	bool shild = false;
	bool autofire = false;
	bool missle = false;

	void draw(int xspos, int yspos);
};

