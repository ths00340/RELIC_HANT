#include "Stage01.h"

void Stage01::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/MissTex02.png");
	size = Float2(500.f, 375.f);
	pos = Float2(SCREEN_WIDTH + (size.x * 0.5f), SCREEN_HEIGHT * 0.5f);
	time = 0;
}