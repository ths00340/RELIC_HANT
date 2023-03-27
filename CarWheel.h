//==============================================================================
// Filename: CarWheel.h
// Description: Driveを継承したホイールクラスの定義
//==============================================================================
#pragma once
#include "Drive.h"
class CarWheel :
	public Drive
{
private:
	Model* model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	Float3 addpos1;
	Float3 addpos2;
	Float3 wheel;
public:
	CarWheel(GameObject* inobject) :Drive(inobject)
	{
		oldPos = inobject->Getpos();
		angle = 0.0f;
		spd = 0.01f;
		move = true;
		addpos1 = Float3(0.0f, 0.0f, 0.0f);
		addpos2 = Float3(0.0f, 0.0f, 0.0f);
		wheel = Float3(0.0f, 0.0f, 0.0f);
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void Load();
	void SetMove(bool Move = false) { move = Move; }
	bool GetMove() { return move; }
	void AddMove(float in) { angle += in; }

	Model* GetModel() { return model; }
};
