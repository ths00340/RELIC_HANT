#include "ResultBackGrownd.h"

void ResultBackGrownd::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/ClearRes.png");
	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	size = Float2(SCREEN_WIDTH, SCREEN_HEIGHT);
}