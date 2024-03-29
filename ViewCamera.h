//==============================================================================
// Filename: ViewCamera.h
// Description :GameObjectクラスを継承した表示用カメラ管理クラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"

class Camera;
class ViewCamera :
	public GameObject
{
private:
	Camera* viewCam = NULL;
	D3DXMATRIX m_ProjectionMatrix;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetView(Camera* incam)
	{
		viewCam = incam;
	}
	Camera* GetView() { return viewCam; };

	bool CheckView(Float3 Position);
};
