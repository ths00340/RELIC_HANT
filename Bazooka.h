//==============================================================================
// Filename: Bazooka.h
// Description: WeponNXp³΅½oY[JNXΜθ`
//==============================================================================
#pragma once
#include "Wepon.h"
class Bazooka :
	public Wepon
{
	static class Model* model;//Cg
	static class Model* hit;//e\ͺf

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	int fire_rate = 0;//­Λ[g

	void Predicted(Float3 spos, Float3 vel);//\ͺp
	Float3 PredictedPoint;//e\ͺΐW
public:
	Bazooka(GameObject* inobject) :Wepon(inobject)
	{
		dmg = 20;
		time = 0;
		angle = 0.0f;
		fire_rate = 0;
		objS = NULL;
		PredictedPoint = Float3(0.0f, 0.0f, 0.0f);
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetDmg(int in) { dmg = in; }
	static void Load();
};
