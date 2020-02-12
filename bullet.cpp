#include "bullet.h"

bool bullet::intersect(asteroid ast)
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