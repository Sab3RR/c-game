#include "mainShip.h"



bool mainShip::Init(int ammunition, int mapw, int maph, float shipmaxspeed, Sprite* ship_sprite, Sprite* bullet_sprite, Sprite* missle_sprite, Sprite* shild_sprite)
{
	sprite = ship_sprite;
	bsprite = bullet_sprite;
	msprite = missle_sprite;
	ssprite = shild_sprite;
	shild = false;
	hasmissle = false;
	autoFire = false;
	getSpriteSize(sprite, size.x, size.y);
	coords.x = mapw / 2;
	coords.y = maph / 2;
	ammo = ammunition;
	maxSpeed = shipmaxspeed;
	boost = 1000;
	curspeed.x = 0;
	curspeed.y = 0;
	inert.x = 0;
	inert.y = 0;
	mapwh.x = mapw;
	mapwh.y = maph;
	trueSpeed = 0;
	return false;
}


void mainShip::draw(int &xspos, int &yspos, int mapx, int mapy, bool sshild)
{
	int screenw;
	int screenh;
	int2 ssize;
	

	getScreenSize(screenw, screenh);
	xspos = coords.x - screenw / 2;
	yspos = coords.y - screenh / 2;
	if (xspos < 0)
		xspos = 0;
	if (yspos < 0)
		yspos = 0;
	if (xspos > mapx - screenw)
		xspos = mapx - screenw;
	if (yspos > mapy - screenh)
		yspos = mapy - screenh;
	drawSprite(sprite, coords.x - size.x / 2 - xspos, coords.y - size.y / 2 - yspos);
	if (sshild)
	{
		getSpriteSize(ssprite, ssize.x, ssize.y);
		drawSprite(ssprite, coords.x - ssize.x / 2 - xspos, coords.y - ssize.y / 2 - yspos);
	}
	for (std::list<bullet>::iterator i = bullets.begin(); i != bullets.end(); i++)
	{
		drawSprite(bsprite, i->coords.x - i->size.x / 2 - xspos, i->coords.y - i->size.x / 2 - yspos);
	}
	for (std::list<missle>::iterator i = missles.begin(); i != missles.end(); i++)
	{
		drawSprite(msprite, i->coords.x - i->size.x / 2 - xspos, i->coords.y - i->size.x / 2 - yspos);
	}
	if (shild)
		drawSprite(ssprite, 0, 0);
	if (autoFire)
		drawSprite(bsprite, 0, 0);
	if (hasmissle)
		drawSprite(msprite, 0, 0);
}

void mainShip::move()
{
	float2 direct;
	float dist;
	clock_t tick;
	
	if (lastTick == 0)
	{
		lastTick = getTickCount();
		return;
	}

	direct.y = (route[(int)FRKey::UP] ? -1 : 0) + (route[(int)FRKey::DOWN] ? 1 : 0);
	direct.x = (route[(int)FRKey::LEFT] ? -1 : 0) + (route[(int)FRKey::RIGHT] ? 1 : 0);
	tick = getTickCount();
	if (direct.x != 0 || direct.y != 0)
	{
		dist = sqrtf(pow(direct.y, 2) + pow(direct.x, 2));
		direct.y /= dist;
		direct.x /= dist;
		inert.x *= trueSpeed;
		inert.y *= trueSpeed;
		inert.x += direct.x * boost * ((tick - lastTick) / (float)1000);
		inert.y += direct.y * boost * ((tick - lastTick) / (float)1000);
		//curspeed.x > maxSpeed ? curspeed.x = maxSpeed : 0;
		//curspeed.y > maxSpeed ? curspeed.y = maxSpeed : 0;
		//curspeed.x < -maxSpeed ? curspeed.x = -maxSpeed : 0;
		//curspeed.y < -maxSpeed ? curspeed.y = -maxSpeed : 0;
		dist = sqrtf(pow(inert.y, 2) + pow(inert.x, 2));
		if (dist)
		{
			inert.x /= dist;
			inert.y /= dist;
		}
		trueSpeed = dist;
		trueSpeed > maxSpeed ? trueSpeed = maxSpeed : 0;
	}
	else {
		inert.x *= trueSpeed;
		inert.y *= trueSpeed;
		if (trueSpeed)
		{
			inert.x += (-inert.x / trueSpeed)* (boost / 4.) * ((tick - lastTick) / (float)1000);
			inert.y += (-inert.y / trueSpeed) * (boost / 4.) * ((tick - lastTick) / (float)1000);
		}
		//(curspeed.x < 1 && curspeed.x > -1) ? curspeed.x = 0 : 0;
		//(curspeed.y < 1 && curspeed.y > -1) ? curspeed.y = 0 : 0;
		dist = sqrtf(pow(inert.y, 2) + pow(inert.x, 2));
		trueSpeed = dist;
		trueSpeed > maxSpeed ? trueSpeed = maxSpeed : 0;
		if (dist < 0.1)
		{
			inert.x = 0;
			inert.y = 0;
		}
		else
		{
			inert.x /= dist;
			inert.y /= dist;
		}
	}
	bmove();
	mmove();
//	printf("speed x - %f , y - %f\n", curspeed.x, curspeed.y);
//	coords.x += curspeed.x * ((tick - lastTick) / 1000.);
//	coords.y += curspeed.y * ((tick - lastTick) / 1000.);
	coords.x += inert.x * (double)trueSpeed * ((tick - lastTick) / (float)1000);
	coords.y += inert.y * (double)trueSpeed * ((tick - lastTick) / (float)1000);
	coords.x < 0 ? coords.x = mapwh.x : 0;
	coords.x > mapwh.x ? coords.x = 0 : 0;
	coords.y < 0 ? coords.y = mapwh.y : 0;
	coords.y > mapwh.y ? coords.y = 0 : 0;
//	printf("coords x - %f , y - %f\n", coords.x, coords.y);

	lastTick = tick;
}

void mainShip::bmove()
{
	std::list<bullet>::iterator i;
	unsigned int Tick;

	Tick = getTickCount();
	for (i = bullets.begin(); i != bullets.end(); i++)
	{
	start:
		if (i != bullets.end())
		{
			if (Tick - i->lifeTick > 5000)
			{
				i = bullets.erase(i);
				goto start;
			}

			i->coords.x += i->direct.x * (double)i->speed * ((Tick - i->lastTick) / (float)1000);
			i->coords.y += i->direct.y * (double)i->speed * ((Tick - i->lastTick) / (float)1000);
			i->coords.x < 0 ? i->coords.x = mapwh.x : 0;
			i->coords.x > mapwh.x ? i->coords.x = 0 : 0;
			i->coords.y < 0 ? i->coords.y = mapwh.y : 0;
			i->coords.y > mapwh.y ? i->coords.y = 0 : 0;
			i->lastTick = Tick;
		}
		else
		{
			break; 
		}
	}
}

bool mainShip::intersect(std::list<asteroid> &ast,std::list<asteroid>::iterator& i)
{
	std::list<asteroid>::iterator j;
	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;

	for (j = ast.begin(); j != ast.end(); j++)
	{

		left = max(coords.x - size.x / 2, j->coords.x - j->size.x / 2);
		top = max(coords.y - size.y / 2, j->coords.y - j->size.y / 2);
		right = min(coords.x + size.x / 2, j->coords.x + j->size.x / 2);
		bottom = min(coords.y + size.y / 2, j->coords.y + j->size.y / 2);

		width = right - left;
		height = bottom - top;

		if (width < 0 || height < 0)
			continue;
		i = j;
		return true;
	
	}
	return false;
}

void mainShip::fire(int x, int y, float bspeed)
{
	bullet new_bullet;
	float d;

	if (bullets.size() == ammo)
		bullets.erase(bullets.begin());
	new_bullet.coords.x = coords.x;
	new_bullet.coords.y = coords.y;
	new_bullet.direct.x = x - coords.x;
	new_bullet.direct.y = y - coords.y;
	d = sqrtf(pow(new_bullet.direct.x, 2) + pow(new_bullet.direct.y, 2));
	new_bullet.direct.x /= d;
	new_bullet.direct.y /= d;
	new_bullet.speed = bspeed;
	new_bullet.lastTick = getTickCount();
	new_bullet.lifeTick = new_bullet.lastTick;
	getSpriteSize(bsprite, new_bullet.size.x, new_bullet.size.y);
	bullets.push_back(new_bullet);
}

void mainShip::autofire(asteroid& ast, float bspeed, float afrange)
{
	float dist;
	float time;
	float2 hit;
	float lastDist = 0;

	dist = sqrtf(pow(coords.x - ast.coords.x, 2) + pow(coords.y - ast.coords.y, 2));
	if (dist > afrange)
		return;
	time = dist / 1000.;
	//printf("dist - %f ; time - %f ; speed - %f\n ", dist, time, ast.speed);
	hit.x = ast.coords.x + ast.direct.x * ast.speed * time;
	hit.y = ast.coords.y + ast.direct.y * ast.speed * time;
	while (lastDist == dist)
	{
		lastDist = dist;
		dist = sqrtf(pow(coords.x - hit.x, 2) + pow(coords.y - hit.y, 2));
		time = dist / 1000.;
		hit.x = ast.coords.x + ast.direct.x * ast.speed * time;
		hit.y = ast.coords.y + ast.direct.y * ast.speed * time;
	}
	fire(hit.x, hit.y, bspeed);
}

void mainShip::swap(asteroid& ast)
{
	float d;
	float speed_sum;
	float diff;
	


	speed_sum =trueSpeed + ast.speed;
	inert.x *= trueSpeed;
	inert.y *= trueSpeed;
	ast.direct.x *= ast.speed;
	ast.direct.y *= ast.speed;
	d = sqrtf(pow(coords.x - (double)ast.coords.x, 2) + pow(coords.y - (double)ast.coords.y, 2));
	d == 0 ? d = 1 : 0;
	inert.x += 100 * (coords.x - ast.coords.x) / d / m;
	inert.y += 100 * (coords.y - ast.coords.y) / d / m;
	ast.direct.x -= 100 * (coords.x - ast.coords.x) / d / ast.m;
	ast.direct.y -= 100 * (coords.y - ast.coords.y) / d / ast.m;
	d = sqrtf(pow(inert.x, 2) + pow(inert.y, 2));
	d == 0 ? d = 1 : 0;
	inert.x = inert.x / d;
	inert.y = inert.y / d;
	trueSpeed = d;
	d = sqrtf(pow(ast.direct.x, 2) + pow(ast.direct.y, 2));
	d == 0 ? d = 1 : 0;
	ast.direct.x = ast.direct.x / d;
	ast.direct.y = ast.direct.y / d;
	ast.speed = d;
	if (trueSpeed + ast.speed > speed_sum)
	{
		diff = (trueSpeed + ast.speed - (float)speed_sum) / (float)2;
		trueSpeed -= diff;
		if (trueSpeed < 0)
		{
			inert.x = -inert.x;
			inert.y = -inert.y;
			trueSpeed = -trueSpeed;
		}
		ast.speed -= diff;
		if (ast.speed < 0)
		{
			ast.direct.x = -ast.direct.x;
			ast.direct.y = -ast.direct.y;
			ast.speed = -ast.speed;
		}
	}
	else if (trueSpeed + ast.speed < speed_sum)
	{
		diff = (speed_sum - trueSpeed - ast.speed) / (float)2;
		trueSpeed += diff;
		ast.speed += diff;
	}

}

void mainShip::misslefollow(std::list<asteroid>::iterator& ast, float mspeed)
{
	missle new_missle;
	float2 dir;
	float dist;

	new_missle.coords.x = coords.x;
	new_missle.coords.y = coords.y;
	dir.x = ast->coords.x - coords.x;
	dir.y = ast->coords.y - coords.y;
	dist = sqrtf(pow(dir.x, 2) + pow(dir.y, 2));
	dist == 0 ? dist = 1 : 0;
	dir.x /= dist;
	dir.y /= dist;
	new_missle.direct.x = dir.x;
	new_missle.direct.y = dir.y;
	new_missle.maxSpeed = mspeed;
	new_missle.speed = new_missle.maxSpeed;
	new_missle.mapwh.x = mapwh.x;
	new_missle.mapwh.y = mapwh.y;
	new_missle.target = ast;
	new_missle.lastTick = getTickCount();
	getSpriteSize(msprite, new_missle.size.x, new_missle.size.y);
	missles.push_back(new_missle);
	hasmissle = false;
}

void mainShip::mmove()
{
	std::list<missle>::iterator i;
	unsigned int Tick;

	Tick = getTickCount();
	for (i = missles.begin(); i != missles.end(); i++)
	{

			i->coords.x += i->direct.x * (double)i->speed * ((Tick - i->lastTick) / (float)1000);
			i->coords.y += i->direct.y * (double)i->speed * ((Tick - i->lastTick) / (float)1000);
			i->coords.x < 0 ? i->coords.x = mapwh.x : 0;
			i->coords.x > mapwh.x ? i->coords.x = 0 : 0;
			i->coords.y < 0 ? i->coords.y = mapwh.y : 0;
			i->coords.y > mapwh.y ? i->coords.y = 0 : 0;
			i->lastTick = Tick;
	}
	
}

void mainShip::aintersect(std::list<ability>& abilities)
{
	std::list<ability>::iterator a;

	int left;
	int top;
	int right;
	int bottom;
	int width;
	int height;
	if (shild || autoFire || hasmissle)
		return;
	for (a = abilities.begin(); a != abilities.end(); a++)
	{

		left = max(coords.x - size.x / 2, a->coords.x - a->size.x / 2);
		top = max(coords.y - size.y / 2, a->coords.y - a->size.y / 2);
		right = min(coords.x + size.x / 2, a->coords.x + a->size.x / 2);
		bottom = min(coords.y + size.y / 2, a->coords.y + a->size.y / 2);

		width = right - left;
		height = bottom - top;

		if (width < 0 || height < 0)
			continue;
		if (a->shild)
			shild = true;
		if (a->autofire)
			autoFire = true;
		if (a->missle)
			hasmissle = true;
		a = abilities.erase(a);
		break;

	}
}
