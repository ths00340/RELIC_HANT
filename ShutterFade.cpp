#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "ShutterFade.h"

ID3D11Buffer* ShutterFade::m_VertexBuffer;//���_�o�b�t�@
ID3D11ShaderResourceView* ShutterFade::m_Texture;//�e�N�X�`���[
ID3D11VertexShader* ShutterFade::m_VertexShader;	//
ID3D11PixelShader* ShutterFade::m_PixelShader;	//�`��p�v���O������ۑ����Ă����p
ID3D11InputLayout* ShutterFade::m_VertexLayout;	//
ID3D11BlendState* ShutterFade::blendState;

void ShutterFade::Init()
{
	FadeParent::Init();
	pos = Float2(SCREEN_WIDTH * 0.5f, -SCREEN_HEIGHT * 0.5f);
#ifndef MUTE
	shutter = Manager::GetCommon()->AddGameObject<Audio>((int)OBJ_LAYER::System);
	shutter->Load("asset\\SE\\shutter.wav");
	shutter->Play(false, 0.1f);
#endif //MUTE
}

void ShutterFade::Uninit()
{
}

void ShutterFade::Update()
{
	FadeParent::Update();
	pos.y = (-SCREEN_HEIGHT * 0.5f) + (SCREEN_HEIGHT * test);
}

void ShutterFade::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(pos.x - (SCREEN_WIDTH * 0.5f), pos.y - (SCREEN_HEIGHT * 0.5f), 0.f);
	vertex[0].Normal = D3DXVECTOR3(0.f, 0.f, 0.f);
	vertex[0].Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	vertex[0].TexCoord = D3DXVECTOR2(0, 0);

	vertex[1].Position = D3DXVECTOR3(pos.x + (SCREEN_WIDTH * 0.5f), pos.y - (SCREEN_HEIGHT * 0.5f), 0.f);
	vertex[1].Normal = D3DXVECTOR3(0.f, 0.f, 0.f);
	vertex[1].Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	vertex[1].TexCoord = D3DXVECTOR2(1, 0);

	vertex[2].Position = D3DXVECTOR3(pos.x - (SCREEN_WIDTH * 0.5f), pos.y + (SCREEN_HEIGHT * 0.5f), 0.f);
	vertex[2].Normal = D3DXVECTOR3(0.f, 0.f, 0.f);
	vertex[2].Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	vertex[2].TexCoord = D3DXVECTOR2(0, 1);

	vertex[3].Position = D3DXVECTOR3(pos.x + (SCREEN_WIDTH * 0.5f), pos.y + (SCREEN_HEIGHT * 0.5f), 0.f);
	vertex[3].Normal = D3DXVECTOR3(0.f, 0.f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	vertex[3].TexCoord = D3DXVECTOR2(1, 1);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	//�ȗ�������ݒ��DISCARD�ɂ��Ă��邽�߁h�̏����ꂽ���Ƃ͖��������h
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void ShutterFade::Load()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(100.0f, -100.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-100.0f, 100.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/FadeTexShutter.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	//�V�F�[�_�[�֌W
	Manager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_OFF);

	blendState = Manager::GetBlend(BLEND_S::SORT_FALSE);
}

void ShutterFade::UnLoad()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}