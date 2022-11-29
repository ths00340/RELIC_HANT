#include "Tools.h"
#include "MenuTitle.h"

void MenuTitle::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/MenuTitle.png");
	size = Float2(SCREEN_WIDTH * 0.27f, SCREEN_HEIGHT * 0.15f);
	pos = Float2(SCREEN_WIDTH * 0.5f, size.y * 0.5f);
	wh = Float2(1.0f, 0.333f);
	defaultpos = pos.y;
}

void MenuTitle::Update()
{
	time++;
	time = TOOL::Limit(time, 20.f, 0.f);

	pos.y = size.y * (0.5f - ((float)time / 10.f));
	if (time == 10)
	{
		uv.y = 0.333f * (float)type;
	}
	int tt = time - 10;
	if (time > 10)
		pos.y = size.y * (-0.5f + ((float)tt / 10.f));
}