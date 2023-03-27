#include "Tools.h"
#include "MissionTex.h"

void MissionTex::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/MissTex0.png");
	size = Float2(500.f, 375.f);
	pos = Float2(SCREEN_WIDTH + (size.x * 0.5f), SCREEN_HEIGHT * 0.5f);
	time = 0;
	name = "MissionTex";
}

void MissionTex::Update()
{
	if (pos.x > SCREEN_WIDTH * 0.5f)
	{
		pos.x -= TOOL::SecDiv(SCREEN_WIDTH * 0.5f);
	}
	else if (time < 1.f)
	{
		time += TOOL::SecDiv(1.f);
	}
	else
	{
		pos.x -= TOOL::SecDiv(SCREEN_WIDTH * 0.5f);
		if (pos.x < -(size.x * 0.5f))
		{
			End = true;
		}
	}
}