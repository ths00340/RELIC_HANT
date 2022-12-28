#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Game.h"
#include "Field.h"
#include "ViewCamera.h"
#include "Camera.h"
#include "Fissure.h"

ID3D11VertexShader* Fissure::m_VertexShader;
ID3D11PixelShader* Fissure::m_PixelShader;
ID3D11InputLayout* Fissure::m_VertexLayout;
ID3D11ShaderResourceView* Fissure::m_Texture;
ID3D11Buffer* Fissure::m_VertexBuffer;
ID3D11BlendState* Fissure::blendState;

void Fissure::Init()
{
	flame = 0;
	maxtime = 60;
	alfa = 1.f;
	m_pos = Float3(0.f, 0.f, 0.f);

	m_pos = Float3(1.f, 1.f, 1.f);
	m_scl = Float3(1.f, 1.f, 1.f);
	m_rot = Float3(0.f, 0.f, 0.f);
	name = "Fissure";
}

void Fissure::Uninit()
{
}

void Fissure::Update()
{
	flame++;
	if (flame > maxtime)
		SetDestroy();

	alfa = 1.f - ((float)flame / (float)maxtime);

	alfa = TOOL::Limit(alfa, 1.f, 0.f);
}

void Fissure::Draw()
{
	if (m_Destoroy)
		return;

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alfa);
	vertex[0].TexCoord = D3DXVECTOR2(0.f, 0.f);

	vertex[1].Position = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alfa);
	vertex[1].TexCoord = D3DXVECTOR2(1.f, 0.f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alfa);
	vertex[2].TexCoord = D3DXVECTOR2(0.f, 1.f);

	vertex[3].Position = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alfa);
	vertex[3].TexCoord = D3DXVECTOR2(1.f, 1.f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//�ȗ�������ݒ��DISCARD�ɂ��Ă��邽�߁h�̏����ꂽ���Ƃ͖��������h

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Fissure::Set(float maxtime)
{
	maxtime = TOOL::FrameMulti(maxtime);
}

void Fissure::Load()
{
	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//�ォ�珑�������\����
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//���_�o�b�t�@�̓��e�����������邱�Ƃ��ł���悤�ɂ���

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	m_Texture = ResourceManager::AddTex("asset/texture/Fis.png");

	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);

	blendState = ResourceManager::GetBlend(BLEND_S::SORT_TRUE);
}

void Fissure::UnLoad()
{
	m_Texture->Release();
	m_VertexBuffer->Release();
}