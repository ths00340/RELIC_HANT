#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "Tools.h"
#include "Game.h"
#include "SphereShadow.h"

ID3D11Buffer* SphereShadow::m_VertexBuffer;

void SphereShadow::Init()
{
	m_Texture = ResourceManager::AddTex("asset/texture/Shadow.png");

	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);
}

void SphereShadow::Uninit()
{
}

void SphereShadow::Update()
{
	m_pos = object->Getpos();
	m_pos.y = 1.01f;
	Float3 vec = object->Getvec();
	vec.y = 0.0f;
	m_pos += vec;

	float growpos = fabsf(object->Getmin().y) * object->Getscl().y;
	float size = growpos + 10.f;
	size = (size - fabsf(object->Getpos().y)) / size;

	size = TOOL::Limit(size, 1.0f, 0.0f);

	m_scl = TOOL::Magni_one(1.0f, object->Getmax().z * object->Getscl().z) * TOOL::Uniform(size);
}

void SphereShadow::Draw()
{
	//省略したら設定でDISCARDにしているため”昔書かれたことは無視される”

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void SphereShadow::Load()
{
	VERTEX_3D vertex[4];
	static Float3 testPos = Float3(0.0f, 0.0f, 0.0f);
	vertex[0].Position = D3DXVECTOR3(testPos.x - 1.0f, testPos.y + 0.0f, testPos.z + 1.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(testPos.x + 1.0f, testPos.y + 0.0f, testPos.z + 1.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(testPos.x - 1.0f, testPos.y - 0.0f, testPos.z - 1.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(testPos.x + 1.0f, testPos.y - 0.0f, testPos.z - 1.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//後から書き換え可能だよ
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点バッファの内容を書き換えることができるようにした

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	ResourceManager::AddTex("asset/texture/Shadow.png");
}

void SphereShadow::UnLoad()
{
	if (m_VertexBuffer != nullptr)
		m_VertexBuffer->Release();
}