#pragma once
#include "Wepon.h"
class Bazooka :
	public Wepon
{
	static Model* model;
	static Model* hit;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

	int fire_rate = 0;

	void Predicted(Float3 spos, Float3 vel);
	Float3 PredictedPoint;
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
