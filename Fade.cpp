#include "manager.h"
#include "Fade.h"
#include "ShutterFade.h"

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
	if (fade == NULL)
		fade = AddGameObject<ShutterFade>((int)OBJ_LAYER::UI);

	Scene::Update();

	if (time == 0)//��������
	{
		OutputDebugString(TEXT("�t�F�[�h�C���J�n\n"));
		fade->SetA(true, false, DestroyTime / 2);
	}
	time++;

	if (time < DestroyTime / 2.0f)//�t�F�[�h�C�����̓���
	{
	}
	else if (time == DestroyTime / 2.0f)//�t�F�[�h�C���I���p����
	{
		OutputDebugString(TEXT("�t�F�[�h�C���I��\n"));
		InFade = true;
		fade->SetA(false, true, DestroyTime / 2);
		OutputDebugString(TEXT("�t�F�[�h�A�E�g�J�n\n"));
	}
	else if (time <= DestroyTime)//�t�F�[�h�A�E�g���̓���
	{
	}
	else//�t�F�[�h�A�E�g�I���p����
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