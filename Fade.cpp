#include "manager.h"
#include "Fade.h"
#include "ShutterFade.h"

ID3D11Buffer* Fade::m_VertexBuffer;//頂点バッファ
ID3D11ShaderResourceView* Fade::m_Texture;//テクスチャー
ID3D11VertexShader* Fade::m_VertexShader;	//
ID3D11PixelShader* Fade::m_PixelShader;	//描画用プログラムを保存しておく用
ID3D11InputLayout* Fade::m_VertexLayout;	//

void Fade::Load()
{
}

void Fade::UnLoad()
{
}

void Fade::Init()
{
	OutputDebugString(TEXT("フェード開始\n"));
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
		OutputDebugString(TEXT("フェードイン終了\n"));
		InFade = true;
		fade->SetA(false, true, DestroyTime / 2);
	}
	else if (time <= DestroyTime)
	{
	}
	else
	{
		OutputDebugString(TEXT("フェードアウト終了\n"));
		OutFade = true;
		Destroy = true;
	}
}

void Fade::Draw()
{
	Scene::Draw();
}