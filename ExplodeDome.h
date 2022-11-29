#pragma once
#include "GameObject.h"
class ExplodeDome :
	public GameObject
{
private:
	static Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;

	class Audio* m_ShotSE;

	float Max = 10.f;
	float endtime = 0.25f;
	int maxdmg = 10;
	bool Decay = false;

	int frame = 0;

	void Finish();

public:
	static void Load();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 pos = Float3(0.0f, 0.0f, 0.0f), float maxrange = 10.f, float inend = 0.25f, int inmaxdmg = 10, bool inde = false);
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
