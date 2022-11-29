#pragma once
#include "GameObject.h"
class ChildTemp :public GameObject
{
private:
	static Model* me;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;

	Float3 m_vel;
	Float3 m_ang_vel;
	float Gravity;
	int live_time;
	int time;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(D3DXVECTOR3 pos, float inGravity, int inLiveTime);
	static void Load();
	static void UnLoad();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};