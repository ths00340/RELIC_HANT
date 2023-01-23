#include "Tools.h"
#include "CreateLogo.h"
#include "ResourceManager.h"

void CreateLogo::Init()
{
	Polygon2D::Init();
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::VIDEO_NOISE);
	Paramarter = { 1.f,10.f,0.f,0.f };
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