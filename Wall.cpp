#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Wall.h"

void Wall::Init()
{
	name = "Field";
	m_pos = Float3(0.0f, 0.0f, 0.0f);
	m_scl = Float3(1.0f, 1.0f, 1.0f);

	numVertex = (CHiP_Y + 1) * (CHiP_X + 1);
	numIndex = ((4 + (2 * (CHiP_X - 1))) * CHiP_Y) + (2 * (CHiP_Y - 1));

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// インデックスバッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	unsigned short* pIdx = (unsigned short*)msr.pData;

	//n枚に対応できるインデックスの設定を考えてみましょう！
	int I = 0;
	int J = 0;
	int count = 0;
	for (int i = 0; i < CHiP_Y; i++)
	{
		for (int j = 0; j < CHiP_X + 1; j++)
		{
			pIdx[I * 2 + 0] = (CHiP_X + 1) + J;
			pIdx[(I * 2) + 1] = J;

			if ((I % 2) == 0)
			{
				Mai[count].a = pIdx[I * 2 + 0];
				Mai[count].b = pIdx[(I * 2) + 1];
			}
			if ((I % 2) == 1)
			{
				Mai[count].c = pIdx[I * 2 + 0];
				Mai[count].d = pIdx[(I * 2) + 1];
				count++;
			}
			I++;
			J++;
		}
		pIdx[(I * 2) + 0] = J - 1;
		pIdx[(I * 2) + 1] = (CHiP_X + 1) + J;
		I++;
	}

	Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);

	{//頂点バッファの中身を埋める
// 頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		//横には2回ループする
		for (int y = 0; y < (CHiP_Y + 1); y++)
		{
			//y_t = rand() % 50;
			//縦には必要枚数+1回ループする
			for (int x = 0; x < (CHiP_X + 1); x++)
			{
				//配列のインデックスを計算する
				int i = y * (CHiP_X + 1) + x;

				pVtx[i].Position = D3DXVECTOR3(0.0f + (x * CHiP_SIZE_X) - ((CHiP_X / 2) * CHiP_SIZE_X), 0.0f - (y * CHiP_SIZE_Y) + ((CHiP_Y / 2) * CHiP_SIZE_X), 0.0f);
				// UV値の設定
				pVtx[i].TexCoord = D3DXVECTOR2(0.0f + x * 4.0f, 0.0f + y * 4.0f);

				// 法線の設定
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// 頂点カラーの設定
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.25f);
			}
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	m_Texture = ResourceManager::AddTex("asset/texture/detline.png");

	//シェーダー関係
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::INVISIBLE_FAR);

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	blendState = ResourceManager::GetBlend(BLEND_S::SORT_FALSE);
}

void Wall::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

void Wall::Update()
{
}

void Wall::Draw()
{
	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
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

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(numIndex, 0, 0);
}