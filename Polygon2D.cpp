#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Polygon2D.h"

void Polygon2D::Init()
{
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
	blendState = Manager::GetBlend(BLEND_S::SORT_FALSE);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	pos = Float2(0.0f, 0.0f);
	size = Float2(100.0f, 100.0f);
	uv = Float2(0.0f, 0.0f);
	wh = Float2(1.0f, 1.0f);
	color = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	rot = 0.0f;
}

void Polygon2D::Uninit()
{
}

void Polygon2D::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = size.x * 0.5f;
	hh = size.y * 0.5f;

	float BaseAngle = atan2f(hh, hw);		// 中心点から頂点に対する角度
	D3DXVECTOR2 temp = D3DXVECTOR2(hw, hh);
	float Radius = D3DXVec2Length(&temp);	// 中心点から頂点に対する距離

	float x = pos.x - cosf(BaseAngle + rot) * Radius;
	float y = pos.y - sinf(BaseAngle + rot) * Radius;
	vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);

	x = pos.x + cosf(BaseAngle - rot) * Radius;
	y = pos.y - sinf(BaseAngle - rot) * Radius;
	vertex[1].Position = D3DXVECTOR3(x, y, 0.0f);

	x = pos.x - cosf(BaseAngle - rot) * Radius;
	y = pos.y + sinf(BaseAngle - rot) * Radius;
	vertex[2].Position = D3DXVECTOR3(x, y, 0.0f);

	x = pos.x + cosf(BaseAngle + rot) * Radius;
	y = pos.y + sinf(BaseAngle + rot) * Radius;
	vertex[3].Position = D3DXVECTOR3(x, y, 0.0f);

	vertex[0].Diffuse = (D3DXVECTOR4)color;
	vertex[1].Diffuse = (D3DXVECTOR4)color;
	vertex[2].Diffuse = (D3DXVECTOR4)color;
	vertex[3].Diffuse = (D3DXVECTOR4)color;

	vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);
	vertex[1].TexCoord = D3DXVECTOR2(uv.x + wh.x, uv.y);
	vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + wh.y);
	vertex[3].TexCoord = D3DXVECTOR2(uv.x + wh.x, uv.y + wh.y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Polygon2D::LoadTex(const char* filename)
{
	m_Texture = Manager::AddTex(filename);
}