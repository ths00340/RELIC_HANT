#include "Tools.h"
#include "Blur_Tex.h"

void Blur_Tex::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/BlurTex.png");
	size = Float2(SCREEN_WIDTH, SCREEN_HEIGHT);
	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	time = 0;
	maxtime = 10;
	draw = false;
	drawIn = false;
	color.a = 0.f;
}

void Blur_Tex::Update()
{
	if (draw)
	{
		time++;
		color.a = ((float)time / (float)maxtime);
		if (time >= maxtime)
			drawIn = true;
	}
	else
	{
		drawIn = false;
		time--;
		color.a = ((float)time / (float)maxtime);
	}
	time = TOOL::Limit(time, maxtime, 0.f);
}

bool Blur_Tex::GetIn()
{
	return drawIn;
}