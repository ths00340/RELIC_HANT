#pragma once
#include "Game.h"
class TextureDrawScene :
	public Game
{
private:
	class Display* DrawTexture;
public:
	void Init()override;
	void Uninit() {};
	void Update() {};
	void Draw();

	void SetShader(SHADER_S inS);
};
