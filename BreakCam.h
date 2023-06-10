//==============================================================================
// Filename: BreakCam.h
// Description: GameObjectクラスを継承したプレイヤー破壊後カメラの定義
//==============================================================================
#pragma once
#include "GameObject.h"

class Camera;
class BreakCam :
	public GameObject
{
private:
	Camera* cam = NULL;
public:
	void Init();
	void Uninit();
	void Update();
	void FixedUpdate();
	void Draw();

	void Set(Float3 inPos, Float3 inUp, Float3 inTag);
};
