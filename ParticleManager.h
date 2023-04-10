//==============================================================================
// Filename: ParticleManager.h
// Description: GameObjectクラスを継承したオブジェクトパーティクル管理クラスの定義
//==============================================================================

//インスタンシングで表示できるようにする
#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "GameObject.h"
#include "ParticleTest.h"

enum class PARTICLE_S
{
	ITEM1,//青
	ITEM2,//オレンジ
	ITEM3,//紫
	GIA,  //ねじ
	DEBRIS,//破片
	CUBE, //正方形
	END,
};

class ParticleManager :
	public GameObject
{
private:
	Model* m_model[(int)PARTICLE_S::END];
	std::list<ParticleTest*> Particle;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* m_BlendState = nullptr;

	ID3D11Buffer* m_pWorldBuffer = NULL;
	ID3D11ShaderResourceView* m_pWorldSRV = NULL;

	Float3 m_vel;
	Float3 startpos;
	int Particles = 0;
	BLEND_S Bstate;
	PARTICLE_S Pstate;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(Float3 pos = Float3(0.0f, 0.0f, 0.0f),
		Float3 rot = Float3(0.f, 0.f, 0.f),
		float vel = 0.0f,
		int particle_num = 1.0f,
		float inGravity = 0.0f,
		int livetime = 2.0f,
		Float3 scl = Float3(0.15f, 0.15f, 0.15f),
		Float3 Rot = Float3(1.0f, 1.0f, 1.0f));

	void SetModel(PARTICLE_S partmodel, BLEND_S blend);


	static void Load();
};
