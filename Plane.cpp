#include "main.h"
#include "renderer.h"
#include "ResourceManager.h"
#include "Plane.h"

void Plane::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position	= Float3(1000.0f, 0.0f, 1000.0f);
	vertex[0].Normal	= Float3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse	= Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= Float2(100.0f, 0.0f);

	vertex[1].Position	= Float3(-1000.0f, 0.0f, 1000.0f);
	vertex[1].Normal	= Float3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse	= Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= Float2(0.0f, 0.0f);

	vertex[2].Position	= Float3(1000.0f, 0.0f, -1000.0f);
	vertex[2].Normal	= Float3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse	= Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= Float2(100.0f, 100.0f);

	vertex[3].Position	= Float3(-1000.0f, 0.0f, -1000.0f);
	vertex[3].Normal	= Float3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse	= Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= Float2(0.0f, 100.0f);


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_pos = Float3(0.f, 0.f, 0.f);
	m_rot = Float3(0.f, 0.0f, 0.f);
	m_scl = Float3(1.f, 1.f, 1.f);

	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);

	blendState = ResourceManager::GetBlend(BLEND_S::SORT_TRUE);
}

void Plane::Uninit()
{
	m_VertexBuffer->Release();
}

void Plane::Update()
{
}

void Plane::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Plane::SetShader(SHADER_S inType)
{
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, inType);
}
