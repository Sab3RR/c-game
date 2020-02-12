#include "missle.h"

void missle::follow(asteroid& ast, std::list<asteroid>& asteroids)
{
	std::list<asteroid>::iterator i;
	float dist;
	float2 hit;
	float lastdist = 0;
	float2 dir;
	float time;

	dir.x = ast.coords.x - coords.x;
	dir.y = ast.coords.y - coords.y;
	dist = sqrtf(pow(dir.x, 2) + pow(dir.y, 2));
	time = dist / maxSpeed;
	hit.x = ast.coords.x + ast.direct.x * ast.speed * time;
	hit.y = ast.coords.y + ast.direct.y * ast.speed * time;
//while (lastdist == dist)
//{
//	lastdist = dist;
//	dist = sqrtf(pow(coords.x - hit.x, 2) + pow(coords.y - hit.y, 2));
//	time = dist / maxSpeed;
//	hit.x = ast.coords.x + ast.direct.x * ast.speed * time;
//	hit.y = ast.coords.y + ast.direct.y * ast.speed * time;
//}
	dist == 0 ? dist = 1 : 0;
	dir.x = hit.x - coords.x + (ast.coords.x - coords.x);
	dir.y = hit.y - coords.y + (ast.coords.y - coords.y);
	dir.x /= dist;
	dir.y /= dist;
	dir.x *= maxSpeed;
	dir.y *= maxSpeed;
	direct.x *= speed;
	direct.y *= speed;
	direct.x += dir.x;
	direct.y += dir.y;
	dist = sqrtf(pow(direct.x, 2) + pow(direct.y, 2));
	dist == 0 ? dist = 1 : 0;
	direct.x /= dist;
	direct.y /= dist;
	speed = dist;
	speed > maxSpeed ? speed = maxSpeed : 0;
}

bool missle::intersect(asteroid& ast)
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

	if (width < 0 || height < 0)
		return false;

	return true;


}