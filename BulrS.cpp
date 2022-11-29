#include "BulrS.h"

void BulrS::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/BlurTexS.png");
	size = Float2(SCREEN_WIDTH, SCREEN_HEIGHT);
	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	time = 0;
	maxtime = 10;
	draw = false;
	drawIn = false;
	color.a = 0.f;
}