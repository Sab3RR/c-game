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
#include "asteroid.h"

class bullet
{
public:
	float2 coords;
	float2 direct;
	int2 size;
	float speed;
	unsigned int lifeTick = 0;
	unsigned int lastTick = 0;
	
	bool intersect(asteroid ast);
};

