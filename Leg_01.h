//==============================================================================
// Filename: Leg_01.h
// Description: Drive‚ðŒp³‚µ‚½‘½‹rƒNƒ‰ƒX‚Ì’è‹`
//==============================================================================

#pragma once
#include "CComponent.h"
#include "Drive.h"
class Leg_01 :
	public Drive
{
private:
	Model* flont;
	Model* Back;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	Float3 addpos;

public:
	Leg_01(GameObject* inobject) :Drive(inobject)
	{
		oldPos = inobject->Getpos();
		angle = 0.0f;
		spd = 0.01f;
		move = true;
		addpos = Float3(0.0f, 0.0f, 0.0f);
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();

	void SetMove(bool Move = false) { move = Move; }
	bool GetMove() { return move; }

	Model* GetModel() { return flont; }
};
