#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "Game.h"
#include "Field.h"
#include "ViewCamera.h"
#include "Camera.h"
#include "Explosion.h"

#define DEFAULT_CHIP_SIZE (100.0f)

ID3D11VertexShader* Explosion::m_VertexShader;
ID3D11PixelShader* Explosion::m_PixelShader;
ID3D11InputLayout* Explosion::m_VertexLayout;
ID3D11ShaderResourceView* Explosion::m_Texture;
ID3D11Buffer* Explosion::m_VertexBuffer;
ID3D11BlendState* Explosion::blendState;

void Explosion::Init()
{
	m_pos = Float3(0.0f, 0.0f, 0.0f);

	tagpos = m_pos;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Count = 0;
	maxcount = 9;
	name = "Explosion";
	flame = 0;
}

void Explosion::Uninit()
{
}

void Explosion::Update()
{
	flame++;
	if (flame >= 1)
	{
		m_Count++;
		flame = 0;
	}

	if (m_Count >= maxcount)
	{
		m_Count = 0;
		SetDestroy();
		return;
	}
}

void Explosion::Draw()
{
	float x = m_Count % maxcount * (1.0f / 9.0f);
	float y = m_Count / maxcount * (1.0f / 1.0f);

	m_pos = tagpos;
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
	vertex[1].TexCoord = D3DXVECTOR2(x + TOOL::Fraction(9.0f), y);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + TOOL::Fraction(9.0f), y + 1.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//�ȗ�������ݒ��DISCARD�ɂ��Ă��邽�߁h�̏����ꂽ���Ƃ͖��������h

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	{
		//�r���[�s��̎擾
		Scene* scene = Manager::GetScene();
		ViewCamera* Vcam = scene->GetGameObject<ViewCamera>();
		D3DXMATRIX view = Vcam->GetView()->GetViewMatrix();

		//�r���[�̋t�s��
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);//�t�s��
		invView._41 = 0.0f;
		invView._42 = 0.0f;
		invView._43 = 0.0f;

		//�}�g���N�X�ݒ�
		D3DXMATRIX world, scl, rot, trans;
		D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
		D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
		world = scl * invView * trans;
		Renderer::SetWorldMatrix(&world);
	}

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f - ((float)m_Count / 9.0f), 1.0f - ((float)m_Count / 9.0f), 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Explosion::Set(Float3 inpos)
{
	tagpos = inpos;
}

void Explosion::Load()
{
	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;//�ォ�珑�������\����
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//���_�o�b�t�@�̓��e�����������邱�Ƃ��ł���悤�ɂ���

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	//�e�N�X�`���ǂݍ���
	m_Texture = Manager::AddTex("asset/texture/fire_.png");

	//�V�F�[�_�[�֌W
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);

	blendState = Manager::GetBlend(BLEND_S::SORT_TRUE);
}

void Explosion::UnLoad()
{
	m_Texture->Release();
	m_VertexBuffer->Release();
}