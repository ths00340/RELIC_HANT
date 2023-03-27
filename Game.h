//==============================================================================
// Filename: Game.h
// Description :Sceneクラスを継承したゲームシーンクラスの定義
//==============================================================================
#pragma once
#include "Scene.h"
#include "BATTLE_DATA.h"

class Game :
	public Scene
{
protected:
	class Audio* bgm = nullptr;
	class Audio* ClearBGM = nullptr;
	class Audio* OverBGM = nullptr;

	class Common* fade = nullptr;
	class Player* pl = nullptr;
	class ViewCamera* VCam = nullptr;

	ID3D11BlendState* m_pDefaultBlend = nullptr;
	BATTLE_DATA* B_Data = nullptr;
	class Blur_Tex* tex = nullptr;

	bool Next = false;
	bool debug_com = false;
	bool WireFrame = false;
	bool Start = false;
	bool Once = false;
	bool Stop = false;

	int cleartime = 0;

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void NoUIDraw();
	virtual void NextScene();
	virtual void BGMSelect();
	virtual void Clear();
	virtual void GameOver();
	void SetPlayer(Player* player);
	void SetNext() { Next = true; }

	template <typename T>
	void SetBattle()
	{
		if (B_Data)
			DeleteBattle();

		B_Data = new T();
		B_Data->Init();
	}

	void DeleteBattle()
	{
		B_Data->Uninit();
		delete B_Data;
		B_Data = nullptr;
	}

	BATTLE_DATA* GetB_Data() { return B_Data; }

#ifndef DEBUG
	void DebugDraw();
#endif // DEBUG
};