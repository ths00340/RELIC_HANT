#pragma once
#include "Scene.h"
#include "BATTLE_DATA.h"

class Game :
	public Scene
{
protected:
	class Audio* bgm = nullptr;
	class Common* fade = nullptr;
	class Player* pl = nullptr;
	class ViewCamera* VCam = nullptr;
	bool Next = false;
	BATTLE_DATA* B_Data = nullptr;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
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