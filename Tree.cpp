#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "ViewCamera.h"
#include "Camera.h"
#include "Tree.h"
ID3D11Buffer* Tree::m_VertexBuffer;

void Tree::Init()
{
	//テクスチャ読み込み
	m_Texture = ResourceManager::AddTex("asset/texture/Tree01.png");

	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);

	blendState = ResourceManager::GetBlend(BLEND_S::SORT_TRUE);

	name = "Tree";
	maxsize = TOOL::Uniform(1.f);
	minsize = TOOL::Uniform(-1.f);
	m_pos = Float3(0.f, 0.f, 0.f);
	m_scl = Float3(8.f, 8.f, 8.f);
	m_rot = Float3(0.f, 0.f, 0.f);
}

void Tree::Uninit()
{
}

void Tree::Update()
{
}

void Tree::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = Float3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = Float3(0.f, 1.0f, 0.0f);
	vertex[0].Diffuse = Float4(1.f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Float2(0, 0);

	vertex[1].Position = Float3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = Float3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Float2(1, 0);

	vertex[2].Position = Float3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = Float3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Float2(0, 1);

	vertex[3].Position = Float3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = Float3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Float2(1, 1);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ビュー行列の取得
	Scene* scene = Manager::GetScene();
	ViewCamera* Vcam = scene->GetGameObject<ViewCamera>();
	D3DXMATRIX view = Vcam->GetView()->GetViewMatrix();

	// ビューの逆行列
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view); // 逆行列

	// Yが1.0f
	invView._21 = 0.0f;
	invView._22 = 1.0f;
	invView._23 = 0.0f;

	D3DXVECTOR3 VectorOfDist = Vcam->GetView()->GetPos() - Getpos();
	D3DXVECTOR3 XVectorOfDist = D3DXVECTOR3(VectorOfDist.x, 0.0f, VectorOfDist.z);
	D3DXVECTOR3 NormalizeDist;
	D3DXVec3Normalize(&NormalizeDist, &XVectorOfDist);
	Float3 CForward = TOOL::VectorNormalize(Vcam->GetView()->GetDir());

	invView._31 = CForward.x;
	invView._32 = CForward.y;
	invView._33 = CForward.z;

	D3DXVECTOR3 CrossDist;
	D3DXVECTOR3 VectorY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 VectorZ = CForward;

	D3DXVec3Cross(&CrossDist, &VectorY, &VectorZ);
	invView._11 = CrossDist.x;
	invView._12 = CrossDist.y;
	invView._13 = CrossDist.z;

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
	material.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Tree::Set(Float3 inpos)
{
	m_pos = inpos;
}

void Tree::Load()
{
	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//後から書き換え可能だよ
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点バッファの内容を書き換えることができるようにした

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	//テクスチャ読み込み
	ResourceManager::AddTex("asset/texture/Tree01.png");
}

void Tree::UnLoad()
{
	if (m_VertexBuffer != nullptr)
		m_VertexBuffer->Release();
}