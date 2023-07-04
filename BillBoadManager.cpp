#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Tools.h"
#include "BillBoadManager.h"

#define MAX_BILLBOAD 500

void BillBoadManager::Init()
{
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::INSTANCE_LIGHT_ON);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//後から書き換え可能だよ
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//頂点バッファの内容を書き換えることができるようにした

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

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

	HRESULT hr;
	D3DXMATRIX* inWorld = DBG_NEW D3DXMATRIX[MAX_BILLBOAD];

	for (int i = 0; i < MAX_BILLBOAD; i++)
	{
		inWorld[i] *= 0.f;
	}

	//Matrix
	{
		// 頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(D3DXMATRIX) * MAX_BILLBOAD;
		bd.StructureByteStride = sizeof(D3DXMATRIX);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = inWorld;

		hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_pWorldBuffer);
		if (FAILED(hr))
		{
			delete[] inWorld;
			int failed = 1;
			TOOL::Display((char*)"エラー：m_pWorldBuffer生成失敗\n");
			return;
		}
		delete[] inWorld;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = MAX_BILLBOAD;
		hr = Renderer::GetDevice()->CreateShaderResourceView(m_pWorldBuffer, &srvd, &m_pWorldSRV);
		if (FAILED(hr))
		{
			int failed = 1;
		}
	}

	name = "ParticleManager";
	m_BlendState = ResourceManager::GetBlend(BLEND_S::SORT_TRUE);

	SetTexture(ResourceManager::AddTex("asset/texture/field004.jpg"));
}

void BillBoadManager::Uninit()
{
	for (D3DXMATRIX matrix : m_Matrixs)
	{
		delete matrix;
	}

	m_Matrixs.clear();

	if (m_VertexBuffer)
		m_VertexBuffer->Release();

	if (m_pWorldBuffer != nullptr)
		m_pWorldBuffer->Release();

	if (m_pWorldSRV != nullptr)
		m_pWorldSRV->Release();
}

void BillBoadManager::Update()
{
	std::vector<D3DXMATRIX> inWorld;
	for (const D3DXMATRIX matrix : m_Matrixs)//範囲forループ
	{
		inWorld.push_back(matrix);
	}
	// 必要な場合にサイズを調整する
	inWorld.resize(MAX_BILLBOAD);
	Renderer::GetDeviceContext()->UpdateSubresource(m_pWorldBuffer, 0, nullptr, inWorld.data(), 0, 0);
}

void BillBoadManager::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー入力
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	Renderer::GetDeviceContext()->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);
	
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
	Renderer::GetDeviceContext()->VSSetShaderResources(3, 1, &m_pWorldSRV);
	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawInstanced(4, m_Matrixs.size(), 0, 0);
}

bool BillBoadManager::Set(Float3 inPos, Float3 inScl)
{
	if (MAX_BILLBOAD<=m_Matrixs.size())
	{
		return false;
	}
	Float3 _pos, _scl, _rot;
	D3DXMATRIX matrix;
	_rot = Float3(0.f, 0.f, 0.f);
	_pos = inPos;
	_scl = inScl;
	//マトリクス設定
	D3DXMATRIX scl, rot, trans;
	D3DXMatrixScaling(&scl, _scl.x, _scl.y, _scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, _rot.y, _rot.x, _rot.z);
	D3DXMatrixTranslation(&trans, _pos.x, _pos.y, _pos.z);
	matrix = scl * rot * trans;

	m_Matrixs.push_back(matrix);
	return true;
}
