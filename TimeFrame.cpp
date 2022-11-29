#include "TimeFrame.h"

void TimeFrame::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/Clock.png");
	size = { 300.f,130.f };
	pos = { SCREEN_WIDTH - 150.f,35.f };
}

void TimeFrame::Uninit()
{
}