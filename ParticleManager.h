#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "GameObject.h"
#include "ParticleTest.h"

enum class PARTICLE_S
{
	ITEM1,//ê¬
	ITEM2,//ÉIÉåÉìÉW
	ITEM3,//éá
	GIA,  //ÇÀÇ∂
	DEBRIS,//îjï–
	CUBE, //ê≥ï˚å`
	END,
};

class ParticleManager :
	public GameObject
{
private:
	static Model* m_model[(int)PARTICLE_S::END];
	std::list<ParticleTest*> Particle;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;

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
		Float3 rot = Float3(0.0f, 0.0f, 0.0f),
		float vel = 0.0f,
		int particle_num = 1.0f,
		float inGravity = 0.0f,
		int livetime = 2.0f,
		Float3 scl = Float3(0.15f, 0.15f, 0.15f),
		Float3 Rot = Float3(1.0f, 1.0f, 1.0f));

	void SetModel(PARTICLE_S partmodel, BLEND_S blend)
	{
		Bstate = blend;
		Pstate = partmodel;
	}

	static void Load();
};
