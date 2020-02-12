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
#include "bullet.h"
#include "missle.h"
#include "struct.h"


class mainShip
{
public:
	int ammo;
	int2 size;
	int2 mapwh;
	bool route[4] = { false, false, false, false };
	float maxSpeed;
	float boost;
	float trueSpeed;
	float m = 1;
	float2 coords;
	float2 curspeed;
	float2 inert;
	Sprite* sprite;
	Sprite* bsprite;
	Sprite* msprite;
	Sprite* ssprite;
	bool hasmissle = false;
	bool shild = false;
	bool autoFire = false;
	std::list<bullet> bullets;
	std::list<missle> missles;
	unsigned int lastTick = 0;

	bool Init(int ammunition, int mapw, int maph, float shipmaxspeed, Sprite* ship_sprite, Sprite* bullet_sprite, Sprite* missle_sprite, Sprite* shild_sprite);
	void draw(int &xspos, int& yspos, int mapx, int mapy, bool sshild);
	void move();
	void bmove();
	void mmove();
	bool intersect(std::list<asteroid> &ast, std::list<asteroid>::iterator& ij);
	void fire(int x, int y, float bspeed);
	void autofire(asteroid& ast, float bspeed, float afrange);
	void swap(asteroid& ast);
	void misslefollow(std::list<asteroid>::iterator& ast, float mspeed);
	void aintersect(std::list<ability>& abilities);


};

