#pragma once
#include "Game.h"
class TextureDrawScene :
	public Game
{
private:
	class Display* DrawTexture;
public:
	void Init();
	void Uninit() {};
	void Update() {};
	void Draw();
};
