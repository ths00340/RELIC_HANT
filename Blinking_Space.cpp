#include "Tools.h"
#include "Blinking_Space.h"

void Blinking_Space::Init()
{
	color.a = 0.0f;
	Polygon2D::Init();
	LoadTex("asset/texture/PushSpace.png");
	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - 100.f);
	size = Float2(600, 200);
}

void Blinking_Space::Update()
{
	a += TOOL::AToR(0.5f);
	color.a = fabsf(cosf(a));
}