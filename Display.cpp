#include "manager.h"
#include "renderer.h"
#include "Display.h"

void Display::Init()
{
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = ResourceManager::GetBlend(BLEND_S::SORT_FALSE);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	pos = Float2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	size = Float2(SCREEN_WIDTH, SCREEN_HEIGHT);
	uv = Float2(0.0f, 0.0f);
	wh = Float2(1.0f, 1.0f);
	color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	rot = 0.0f;
}