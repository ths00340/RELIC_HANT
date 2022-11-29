#include "Stage02.h"

void Stage02::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/MissTex01.png");
	size = Float2(500.f, 375.f);
	pos = Float2(SCREEN_WIDTH + (size.x * 0.5f), SCREEN_HEIGHT * 0.5f);
	time = 0;
}