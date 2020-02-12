#pragma once
#include <Windows.h>
#include <Framework.h>
#include <utility>
#include <stdint.h>
#include <cstdlib>
#include <list>
#include "asteroid.h"
#include "struct.h"

class missle
{
public:
	float2 coords;
	float2 direct;
	float speed;
	float maxSpeed;
	int2 mapwh;
	int2 size;
	std::list<asteroid>::iterator target;
	unsigned int lastTick;

	bool intersect(asteroid& ast);
	void follow(asteroid& ast, std::list<asteroid>& asteroids);
};

