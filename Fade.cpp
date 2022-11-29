#include "manager.h"
#include "Fade.h"
#include "ShutterFade.h"

ID3D11Buffer* Fade::m_VertexBuffer;//���_�o�b�t�@
ID3D11ShaderResourceView* Fade::m_Texture;//�e�N�X�`���[
ID3D11VertexShader* Fade::m_VertexShader;	//
ID3D11PixelShader* Fade::m_PixelShader;	//�`��p�v���O������ۑ����Ă����p
ID3D11InputLayout* Fade::m_VertexLayout;	//

void Fade::Load()
{
}

void Fade::UnLoad()
{
}

void Fade::Init()
{
	OutputDebugString(TEXT("�t�F�[�h�J�n\n"));
	time = 0;
	InFade = false;
	OutFade = false;
}

void Fade::Uninit()
{
	Scene::Uninit();
}

void Fade::Update()
{
	Scene::Update();
	if (fade == NULL)
		fade = AddGameObject<ShutterFade>((int)OBJ_LAYER::UI);

	Scene::Update();
	if (time == 0)
		fade->SetA(true, false, DestroyTime / 2);

	time++;
	if (time < DestroyTime / 2.0f)
	{
	}
	else if (time == DestroyTime / 2.0f)
	{
		OutputDebugString(TEXT("�t�F�[�h�C���I��\n"));
		InFade = true;
		fade->SetA(false, true, DestroyTime / 2);
	}
	else if (time <= DestroyTime)
	{
	}
	else
	{
		OutputDebugString(TEXT("�t�F�[�h�A�E�g�I��\n"));
		OutFade = true;
		Destroy = true;
	}
}

void Fade::Draw()
{
	Scene::Draw();
}