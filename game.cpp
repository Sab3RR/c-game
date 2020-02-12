
#include <iostream>
#include <Windows.h>
#include <Framework.h>
#include <utility>
#include <cstddef>
#include <cstdlib>
#include <stdint.h>
#include <list>
#include <ctime>
#include "struct.h"
#include "mainShip.h"
#include "background.h"
#include "asteroid.h"
#include "ability.h"


/* Test Framework realization */
class MyFramework : public Framework {


private:
	mainShip ship;
	std::list<asteroid> asteroids;
	background back;
	Sprite* ast_sprite = 0;
	Sprite* small_ast_sprite = 0;
	Sprite* ship_sprite = 0;
	Sprite* bullet_sprite = 0;
	Sprite* missle_sprite = 0;
	Sprite* sability = 0;
	Sprite* shild_sprite = 0;
	int _xspos = 0;
	int _yspos = 0;
	int _x = 0;
	int _y = 0;
	int _xrelative = 0;
	int _yrelative = 0;
	std::list<ability> abilities;
	unsigned int autofiretick = 0;
	unsigned int shildtick = 0;

	std::list<asteroid>::iterator getAsteroid()
	{
		int left;
		int top;
		int right;
		int bottom;
		int width;
		int height;
		std::list<asteroid>::iterator i;

		for (i = asteroids.begin(); i != asteroids.end(); i++)
		{
			

			left = max(i->coords.x - i->size.x / 2, _x + _xspos - 1);
			top = max(i->coords.y - i->size.y / 2, _y + _yspos - 1);
			right = min(i->coords.x + i->size.x / 2, _x + _xspos + 1);
			bottom = min(i->coords.y + i->size.y / 2, _y + _yspos + 1);

			width = right - left;
			height = bottom - top;

			if (width > 0 && height > 0)
				return i;
		}
		return i;
	}

public:
	int w = 800;
	int h = 600;
	int mapw = 1000;
	int maph = 1000;
	int ast = 10;
	int ammo = 3;
	float ability_probability = 0.3;
	int hole = 300;
	float afrange = 300;
	float force = 10;
	float astmaxspeed = 300;
	float shipmaxspeed = 300;
	float bspeed = 1000;
	float mspeed = 300;
	bool asteroids_crash = false;
	bool godmode = false;
	bool autofire = false;
	bool infinityautofire = false;
	bool shild = false;
	bool infinityshild = false;
	bool infinitymissle = false;
	bool fs = false;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = w;
		height = h;
		fullscreen = fs;
	}

	virtual bool Init() {
		asteroid astinit;
		int i = 0;
		ship_sprite = createSprite("data/spaceship.png");
		if (ship_sprite == NULL)
		{
			return false;
		}
		bullet_sprite = createSprite("data/bullet.png");
		if (bullet_sprite == NULL)
		{
			return false;
		}
		missle_sprite = createSprite("data/enemy.png");
		if (missle_sprite == NULL)
		{
			return false;
		}
		shild_sprite = createSprite("data/circle.tga");
		if (shild_sprite == NULL)
		{
			return false;
		}
		ship.Init(ammo, mapw, maph, shipmaxspeed, ship_sprite, bullet_sprite, missle_sprite, shild_sprite);
		back.Init();
		ast_sprite = createSprite("data/big_asteroid.png");
		if (ast_sprite == NULL)
		{
			return false;
		}
		small_ast_sprite = createSprite("data/small_asteroid.png");
		if (small_ast_sprite == NULL)
		{
			return false;
		}
		sability = createSprite("data/reticle.png");
		if (sability == NULL)
		{
			return false;
		}
		for (i; i < ast; i++)
		{
			astinit.Init(ship.coords, mapw, maph, hole, astmaxspeed, force, ability_probability, ast_sprite, small_ast_sprite, sability);
			asteroids.push_back(astinit);
		}
	//	drawSprite(ship.sprite, 0, 0);
		printf("w - %i  h - %i screen\n", w, h);
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
      //  drawTestBackground();
		std::list<asteroid>::iterator j;
		std::list<asteroid>::iterator k;
		std::list<missle>::iterator m;
		std::list<bullet>::iterator l;
		std::list<ability>::iterator a;
		int2 square;
		asteroid new_ast;
		bool ast1;
		bool ast2;
		float dist;
		

		if (getTickCount() - shildtick > 10000)
				shild = false;
		if ((ship.intersect(asteroids, j) && !godmode) || asteroids.size() == 0)
		{
			if ((shild && asteroids.size() != 0) || (infinityshild && asteroids.size() != 0))
			{
				ship.swap(*j);
			}
			else
			{
				if (asteroids.size() == 0)
					printf("You won! Congratulations\n");
				else
					printf("You lose! What a shame\n");
				shild = false;
				autofire = false;
				ship.Init(ammo, mapw, maph, shipmaxspeed, ship_sprite, bullet_sprite, missle_sprite, shild_sprite);
				j = asteroids.begin();
				while (j != asteroids.end())
					j = asteroids.erase(j);
				for (int i = 0; i < ast; i++)
				{
					new_ast.Init(ship.coords, mapw, maph, hole, astmaxspeed, force, ability_probability, ast_sprite, small_ast_sprite, sability);
					asteroids.push_back(new_ast);
				}
				l = ship.bullets.begin();
				while (l != ship.bullets.end())
				{
					l = ship.bullets.erase(l);
				}
				m = ship.missles.begin();
				while (m != ship.missles.end())
				{
					m = ship.missles.erase(m);
				}
				a = abilities.begin();
				while (a != abilities.end())
				{
					a = abilities.erase(a);
				}
				return false;
			}
		}
		j = asteroids.begin();
		while ( j != asteroids.end())
		{
			start:
			l = ship.bullets.begin();
			while (l != ship.bullets.end() && j != asteroids.end())
			{
				if (l->intersect(*j))
				{
					ship.bullets.erase(l);
					m = ship.missles.begin();
					while (m != ship.missles.end())
					{
					
						if (m->target == j)
							m = ship.missles.erase(m);
						else
							m++;
					}
					j->destroy(asteroids, j, abilities);
					l = ship.bullets.begin();
				}
				else
					l++;
			}
			m = ship.missles.begin();
			while (m != ship.missles.end() && j != asteroids.end())
			{
				if (m->intersect(*j))
				{
					ship.missles.erase(m);
					m = ship.missles.begin();
					while (m != ship.missles.end())
					{

						if (m->target == j)
							m = ship.missles.erase(m);
						else
							m++;
					}
					j->destroy(asteroids, j, abilities);
					m = ship.missles.begin();
				}
				else
					m++;
			}
			k = asteroids.begin();
			while (k != asteroids.end())
			{
				if (k == j || j == asteroids.end())
				{
					k++;
					continue;
				}
				if (j->intersect(*k, square))
				{
					ast1 = (square.x * square.y / (k->size.x * k->size.y) > 0.5 && asteroids_crash);
					ast2 = (square.x * square.y / (j->size.x * j->size.y) > 0.5 && asteroids_crash);
					if (ast1)
					{
						m = ship.missles.begin();
						while (m != ship.missles.end())
						{

							if (m->target == k)
								m = ship.missles.erase(m);
							else
								m++;
						}
						k->destroy(asteroids, k, abilities);
					}
					if (ast2)
					{
						m = ship.missles.begin();
						while (m != ship.missles.end())
						{

							if (m->target == k)
								m = ship.missles.erase(m);
							else
								m++;
						}
						j->destroy(asteroids, j, abilities);
						if (j == asteroids.end())
							break;
						goto start;
					}
					if (!ast1 && !ast2)
					{
						j->swap(*k);
						k++;
					}
				}
				else
					k++;
			}
			if (j == asteroids.end())
				break;
			j->move();
			j++;
		}
		for (m = ship.missles.begin(); m != ship.missles.end(); m++)
		{
			
			m->follow(*m->target, asteroids);
		}
		if (autofire || infinityautofire)
		{
			if (getTickCount() - autofiretick > 10000)
				autofire = false;
			if (ship.bullets.size() < ship.ammo)
			{
				k = asteroids.end();
				dist = INFINITE;
				for (j = asteroids.begin(); j != asteroids.end(); j++)
				{
					if (abs(ship.coords.x - j->coords.x) + abs(ship.coords.y - j->coords.y) < dist)
					{
						dist = abs(ship.coords.x - j->coords.x) + abs(ship.coords.y - j->coords.y);
						k = j;
					}
				}
				if (k != asteroids.end())
					ship.autofire(*k, bspeed, afrange);
			}
		}
		
		ship.aintersect(abilities);
		ship.move();
		back.draw();
		
		ship.draw(_xspos, _yspos, mapw, maph, (shild || infinityshild));
		for (j = asteroids.begin(); j != asteroids.end(); j++)
			j->draw(_xspos, _yspos);
		for (a = abilities.begin(); a != abilities.end(); a++)
			a->draw(_xspos, _yspos);
	//	system("CLS");
	//	printf("asteroids size %i\n", asteroids.size());
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		_x = x;
		_y = y;
		_xrelative = xrelative;
		_yrelative = yrelative;
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (FRMouseButton::LEFT == button && isReleased == false)
		{
			ship.fire(_x + _xspos, _y + _yspos, bspeed);
		}
		else if (FRMouseButton::RIGHT == button && isReleased == false)
		{
			std::list<asteroid>::iterator target;
			if (ship.shild)
			{
				shild = true;
				ship.shild = false;
				shildtick = getTickCount();
			}
			if (ship.autoFire)
			{
				autofire = true;
				ship.autoFire = false;
				autofiretick = getTickCount();
			}
			if (ship.hasmissle || infinitymissle)
			{
				target = getAsteroid();
				if (target != asteroids.end())
				{
					ship.misslefollow(target, mspeed);
				}
			}
		}
	}

	virtual void onKeyPressed(FRKey k) {

		ship.route[(int)k] = true;
	}

	virtual void onKeyReleased(FRKey k) {
		ship.route[(int)k] = false;
	}
	
	virtual const char* GetTitle() override
	{
		return "asteroids";
	}
};

int main(int argc, char *argv[])
{
	int width = 0;
	int height = 0;
	int mapWidth = 0;
	int mapHeight = 0;
	int asteroids = 0;
	float ability = -1;
	int ammo = 0;
	int hole = 0;
	float afrange = 0;
	float force = 0;
	float astmaxspeed = 0;
	float shipmaxspeed = 0;
	float bspeed = 0;
	float mspeed = 0;
	bool asteroids_crash = 0;
	bool godmode = 0;
	bool infinityautofire = 0;
	bool infinityshild = 0;
	bool infinitymissle = 0;
	bool fullscreen = 0;
	int i = 1;
	int j;
	bool error = false;
	MyFramework* frame = new MyFramework;
	//std::cout << argv[0] << std::endl;
	if (argc > 1)
	{
		while (i < argc)
		{
			if (strcmp(argv[i], "-window") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				j = 0;
				if ((width = atoi(argv[i])) < 200)
				{
					error = true;
					goto endinit;
				}
				while (*(argv[i] + j))
				{
					if (*(argv[i] + j) == 'x' || *(argv[i] + j) == 'X')
						break;
					j++;
				}
				if ((height = atoi(argv[i] + j + 1)) < 200)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-map") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				j = 0;
				if ((mapWidth = atoi(argv[i])) < 200)
				{
					error = true;
					goto endinit;
				}
				while (*(argv[i] + j))
				{
					if (*(argv[i] + j) == 'x' || *(argv[i] + j) == 'X')
						break;
					j++;
				}
				if ((mapHeight = atoi(argv[i] + j + 1)) < 200)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-num_asteroids") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((asteroids = atoi(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-num_ammo") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((ammo = atoi(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-ability_probability") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((ability = atof(argv[i])) < 0 || ability > 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-hole") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((hole = atoi(argv[i])) < 100)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-autofire_range") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((afrange = atof(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-force") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((force = atof(argv[i])) <= 0)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-asteroid_maxspeed") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((astmaxspeed = atof(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-ship_maxspeed") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((shipmaxspeed = atof(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-bullet_speed") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((bspeed = atof(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-missle_speed") == 0)
			{
				i++;
				if (!(i < argc))
				{
					error = true;
					goto endinit;
				}
				if ((mspeed = atof(argv[i])) < 1)
				{
					error = true;
					goto endinit;
				}
				i++;
			}
			else if (strcmp(argv[i], "-asteroid_crash") == 0)
			{
				
				asteroids_crash = true;
				i++;
			}
			else if (strcmp(argv[i], "-godmode") == 0)
			{
				godmode = true;
				i++;	
			}
			else if (strcmp(argv[i], "-infinity_autofire") == 0)
			{
				infinityautofire = true;
				i++;
			}
			else if (strcmp(argv[i], "-infinity_shild") == 0)
			{
				infinityshild = true;
				i++;
			}
			else if (strcmp(argv[i], "-infinity_missle") == 0)
			{
				infinitymissle = true;
				i++;
			}
			else if (strcmp(argv[i], "-fullscreen") == 0)
			{
				fullscreen = true;
				i++;
			}
			else
			{
				error = true;
				goto endinit;
			}

		}
	}
endinit:
	if (error == true)
	{
		printf("MANUAL: game.exe -window 1200x720 -map 5000x5020 -asteroid_crash -godmode -num_asteroids 1000 -num_ammo 100 -ability_probability 0.1  -asteroid_maxspeed 300 -infinity_missle -infinity_autofire -infinity_shild -ship_maxspeed 5000 -bullet_speed 1000 -missle_speed 5000 -hole 100 -force 10 -autofire_range 300\n");
		return 0;
	}
	if (width != 0 && height != 0)
	{
		frame->w = width;
		frame->h = height;
	}
	if (mapWidth != 0 && mapHeight != 0)
	{
		frame->mapw = mapWidth;
		frame->maph = mapHeight;
	}
	if (asteroids != 0)
		frame->ast = asteroids;
	if (ammo != 0)
		frame->ammo = ammo;
	if (ability >= 0 && ability <= 1)
		frame->ability_probability = ability;
	if (afrange != 0)
		frame->afrange = afrange;
	if (force != 0)
		frame->force = force;
	if (astmaxspeed != 0)
		frame->astmaxspeed = astmaxspeed;
	if (shipmaxspeed != 0)
		frame->shipmaxspeed = shipmaxspeed;
	if (bspeed != 0)
		frame->bspeed = bspeed;
	if (mspeed != 0)
		frame->mspeed = mspeed;
	frame->asteroids_crash = asteroids_crash;
	frame->godmode = godmode;
	frame->infinityautofire = infinityautofire;
	frame->infinityshild = infinityshild;
	frame->infinitymissle = infinitymissle;
	frame->fs = fullscreen;
	if (frame->w > frame->mapw || frame->h > frame->maph)
	{
		printf("map can not be smaller then window\n");
		return 0;
	}
	return run(frame);
}
