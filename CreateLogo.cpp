#include "Tools.h"
#include "CreateLogo.h"

void CreateLogo::Init()
{
	Polygon2D::Init();
	LoadTex("asset/texture/SoloLogo.png");
	color.a = 0.0f;
	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	size = Float2(800.f, 800.f);
}

void CreateLogo::Update()
{
	color.a += TOOL::SecDiv(1.f);
	color.a = TOOL::Limit(color.a, 1.0f, 0.0f);
}