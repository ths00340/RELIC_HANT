#pragma once
#include "GameObject.h"

class GameobjTemp :public GameObject
{
private:
	static Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;

	class Audio* m_ShotSE;
public:
	static void Load();
	static void UnLoad();
	void Init();
	void Uninit();
	void Update();
	void FixedUpdate();
	void Draw();
	ID3D11BlendState* GetBlendState()override { return blendState; }
};
