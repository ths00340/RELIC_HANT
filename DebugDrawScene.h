#pragma once
#include "Game.h"
class DebugDrawScene :
	public Game
{
public:
	void Init() {
		view.Width = (FLOAT)SCREEN_WIDTH / 4;
		view.Height = (FLOAT)SCREEN_HEIGHT / 4;
		view.MinDepth = 0.0f;
		view.MaxDepth = 1.0f;
		view.TopLeftX = 0;
		view.TopLeftY = 0;
	};
	void Uninit() {};
	void Draw();
	void Update() {};
};
