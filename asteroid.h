#pragma once

#include <Windows.h>
#include <Framework.h>
#include <utility>
#include <stdint.h>
#include <cstdlib>
#include <list>
#include "struct.h"
#include "ability.h"


class asteroid
{
public:
	Sprite* sprite;
	Sprite* smallSprite;
	Sprite* sability;
	float2 coords;
	float2 direct;
	float speed;
	float maxSpeed;
	int2 mapwh;
	int2 size;
	bool issmall;
	float probability = 0;
	float m;
	float f;
	unsigned int lastTick = 0;
	
	bool Init(float2 shipCoords, int mapw, int maph, int hole, float mspeed, float force, float ability_probability, Sprite* asteroid, Sprite* small_ast, Sprite* s_ability);
	void move();
	void draw(int xspos, int yspos);
	bool intersect(asteroid& ast, int2& square);
	void swap(asteroid& ast);
	void destroy(std::list<asteroid>& list, std::list<asteroid>::iterator& ast, std::list<ability>& abilities);
};

