#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Game.h"
#include "ViewCamera.h"
#include "Camera.h"
#include "ChargeEffect.h"

ID3D11Buffer* ChargeEffect::m_VertexBuffer;

void ChargeEffect::Init()
{
	m_Texture = ResourceManager::AddTex("asset/texture/charge_bru.png");
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);

	blendState = ResourceManager::GetBlend(BLEND_S::SORT_TRUE);
	m_pos = Float3(0.f, 0.f, 0.f);

	m_pos = Float3(0.f, 0.f, 0.f);
	m_scl = Float3(1.f, 1.f, 1.f);
	m_rot = Float3(0.f, 0.f, 0.f);

	m_Count = 0;
	maxcount = 10;
	name = "ChargeEffect";
}

void ChargeEffect::Uninit()
{
}

void ChargeEffect::Update()
{
	flame++;
	if (flame >= flametime)
	{
		m_Count++;
		flame = 0;
	}

	if (m_Count > maxcount)
	{
		m_Count = maxcount;
		SetDestroy();
		return;
	}
}

void ChargeEffect::Draw()
{
	if (m_Destoroy)
		return;

	float x = m_Count % maxcount * (1.0f / 10.0f);
	float y = m_Count / maxcount * (1.0f / 1.0f);
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + TOOL::Fraction(10.0f), y);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + TOOL::Fraction(10.0f), y + 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//省略したら設定でDISCARDにしているため”昔書かれたことは無視される”

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ビュー行列の取得
	Scene* scene = Manager::GetScene();
	ViewCamera* Vcam = scene->GetGameObject<ViewCamera>();
	D3DXMATRIX view = Vcam->GetView()->GetViewMatrix();

	//ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);//逆行列
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * invView * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
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

void ChargeEffect::Set(float maxtime)
{
	int frame = 0;
	flame = TOOL::FrameMulti(maxtime);
	TOOL::Display((char*)"再生フレーム%d:1\n", flametime);
	flametime = flame / 10;
	int add = flametime * 10;
	add = flame - add;
	if (fabs(add) > 0)
		flametime += 1;

	TOOL::Display((char*)"平均フレーム差%d:\n", fabs(add));
}

void ChargeEffect::Load()
{
	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//後から書き換え可能だよ
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点バッファの内容を書き換えることができるようにした

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);
	ResourceManager::AddTex("asset/texture/charge_bru.png");
}

void ChargeEffect::UnLoad()
{
	if (m_VertexBuffer != nullptr)
		m_VertexBuffer->Release();
}