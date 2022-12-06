#include "Polygon2D.h"
#include "Tools.h"
#include "Title2D.h"

void Title2D::Init()
{
	updown = 0.f;
	Polygon2D::Init();
	LoadTex("asset/texture/TitleTex.png");
	defaultpos = pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.2f);
	size = Float2(730.f, 500.f);
}

void Title2D::Update()
{
	updown += TOOL::AToR(0.8f);
	pos = Float2(defaultpos.x, defaultpos.y + (20.f * cosf(updown)));
}