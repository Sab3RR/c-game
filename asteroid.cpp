#include "asteroid.h"

bool asteroid::Init(float2 shipCoords, int mapw, int maph, int hole, float mspeed, float force, float ability_probability,  Sprite* asteroid, Sprite* small_asteroid, Sprite* s_ability)
{
	float dist;
	sprite = asteroid;
	smallSprite = small_asteroid;
	sability = s_ability;
	probability = ability_probability;
	issmall = false;
	m = 1;
	f = force;
	mapwh.x = mapw;
	mapwh.y = maph;
	maxSpeed = mspeed;
	if (rand() % 2)
		coords.x = (int)(shipCoords.x + hole) + rand() % (int)(mapwh.x - shipCoords.x - hole);
	else
		coords.x = rand() % (int)(shipCoords.x - hole);
	
	if (rand() % 2)
		coords.y = (int)(shipCoords.y + hole) + rand() % (int)(mapwh.y - shipCoords.y - hole);
	else
		coords.y = rand() % (int)(shipCoords.y - hole);
	speed = abs((rand() % 8000) / 100.0 + 20);
	if (rand() % 2)
		direct.x = rand();
	else
		direct.x = rand() * -1;
	if (rand() % 2)
		direct.y = rand();
	else
		direct.y = rand() * -1;
	dist = sqrtf(pow(direct.x, 2) + pow(direct.y, 2));
	direct.x = direct.x / dist;
	direct.y = direct.y / dist;
	getSpriteSize(sprite, size.x, size.y);
	return false;
}

void asteroid::move()
{
	unsigned int tick;

	if (lastTick == 0)
	{
		lastTick = getTickCount();
		return;
	}
	tick = getTickCount();
	speed > maxSpeed ? speed = maxSpeed : 0;
	coords.x += direct.x * speed * ((tick - lastTick) / (float)1000);
	coords.y += direct.y * speed * ((tick - lastTick) / (float)1000);
	if (coords.x < 0)
		coords.x = mapwh.x;
	else if (coords.x > mapwh.x)
		coords.x = 0;
	if (coords.y < 0)
		coords.y = mapwh.y;
	else if (coords.y > mapwh.y)
		coords.y = 0;
	lastTick = tick;
}

void asteroid::draw(int xspos, int yspos)
{
	if (issmall)
	{
		drawSprite(smallSprite, coords.x - size.x / 2 - xspos, coords.y - size.y / 2 - yspos);
	}
	else
		drawSprite(sprite, coords.x - size.x / 2 - xspos, coords.y - size.y / 2 - yspos);
}

bool asteroid::intersect(asteroid& ast, int2 &square)
{
	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;

	left = max(coords.x - size.x / 2, ast.coords.x - ast.size.x / 2);
	top = max(coords.y - size.y / 2, ast.coords.y - ast.size.y / 2);
	right = min(coords.x + size.x / 2, ast.coords.x + ast.size.x / 2);
	bottom = min(coords.y + size.y / 2, ast.coords.y + ast.size.y / 2);

	width = right - left;
	height = bottom - top;

	square.x = width;
	square.y = height;
	if (width < 0 || height < 0)
		return false;

	return true;


}

void asteroid::swap(asteroid& ast)
{
	float d;
	float speed_sum;
	float diff;
	

	speed_sum = speed + ast.speed;
	direct.x *= speed;
	direct.y *= speed;
	ast.direct.x *= ast.speed;
	ast.direct.y *= ast.speed;
	d = sqrtf(pow(coords.x - (double)ast.coords.x, 2) + pow(coords.y - (double)ast.coords.y, 2));
	d == 0 ? d = 1 : 0;
	direct.x += f *(coords.x - ast.coords.x) / d / m;
	direct.y += f *(coords.y - ast.coords.y) / d / m;
	ast.direct.x -= f *(coords.x - ast.coords.x) / d / ast.m;
	ast.direct.y -= f *(coords.y - ast.coords.y) / d / ast.m;
	d = sqrtf(pow(direct.x, 2) + pow(direct.y, 2));
	d == 0 ? d = 1 : 0;
	direct.x = direct.x / d;
	direct.y = direct.y / d;
	speed = d;
	d = sqrtf(pow(ast.direct.x, 2) + pow(ast.direct.y, 2));
	d == 0 ? d = 1 : 0;
	ast.direct.x =ast.direct.x / d;
	ast.direct.y =ast.direct.y / d;
	ast.speed = d;
	//if (speed + ast.speed > speed_sum)
	//{
	//	diff = (speed + ast.speed - (float)speed_sum) / (float)2;
	//	speed -= diff;
	//	if (speed < 0)
	//	{
	//		direct.x = -direct.x;
	//		direct.y = -direct.y;
	//		speed = -speed;
	//	}
	//	ast.speed -= diff;
	//	if (ast.speed < 0)
	//	{
	//		ast.direct.x = -ast.direct.x;
	//		ast.direct.y = -ast.direct.y;
	//		ast.speed = -ast.speed;
	//	}
	//}
	//else if (speed + ast.speed < speed_sum)
	//{
	//	diff = (speed_sum - speed - ast.speed) / (float)2;
	//	speed += diff;
	//	ast.speed += diff;
	//}
	
}

void asteroid::destroy(std::list<asteroid>& list, std::list<asteroid>::iterator& ast, std::list<ability>& abilities)
{
	asteroid newAsteroid;
	ability newability;
	int chance;
	double angle = 0.785398;

	if (issmall)
	{
		ast = list.erase(ast);
		return;
	}
	newAsteroid.issmall = true;
	newAsteroid.smallSprite = ast->smallSprite;
	getSpriteSize(newAsteroid.smallSprite, newAsteroid.size.x, newAsteroid.size.y);
	newAsteroid.coords.x = coords.x + -direct.y * (float)newAsteroid.size.x;
	newAsteroid.coords.y = coords.y + direct.x * (float)newAsteroid.size.y;

	newAsteroid.direct.x = direct.x * cos(angle) - direct.y * sin(angle);
	newAsteroid.direct.y = direct.x * sin(angle) + direct.y * cos(angle);
	newAsteroid.mapwh.x = mapwh.x;
	newAsteroid.mapwh.y = mapwh.y;
	newAsteroid.m = newAsteroid.size.x * (float)newAsteroid.size.y / (size.x * (float)size.y);
	newAsteroid.speed = speed;
	newAsteroid.maxSpeed = maxSpeed;
	newAsteroid.lastTick = getTickCount();
	list.push_back(newAsteroid);
	newAsteroid.direct.x = direct.x * cos(-angle) - direct.y * sin(-angle);
	newAsteroid.direct.y = direct.x * sin(-angle) + direct.y * cos(-angle);
	newAsteroid.coords.x = direct.y * (float)newAsteroid.size.x + (float)coords.x;
	newAsteroid.coords.y = -direct.x * (float)newAsteroid.size.y + (float)coords.y;
	
	list.push_back(newAsteroid);
	newability.coords.x = coords.x;
	newability.coords.y = coords.y;
	getSpriteSize(sability, newability.size.x, newability.size.y);
	chance = rand() % 100;
	if (chance > 0 && chance < 100 / 3)
		newability.shild = true;
	else if (chance > (100 / 3) && chance < (100 / 3) * 2)
		newability.autofire = true;
	else
		newability.missle = true;
	newability.sprite = sability;
	if (rand() / (float)RAND_MAX < probability)
		abilities.push_back(newability);
	
	ast = list.erase(ast);


}