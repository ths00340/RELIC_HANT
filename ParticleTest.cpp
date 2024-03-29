#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "Scene.h"
#include "ParticleTest.h"
#include "Tools.h"

void ParticleTest::Init()
{
	name = "ParticleTest";
	time = 0;
	m_pos = Float3(-3.f, 1.f, 0.f);
	m_scl = Float3(0.15f, 0.15f, 0.15f);
	DefaultSize = m_scl;
	m_rot = Float3(0.f, 0.f, 0.f);
	m_ang_vel = Float3
	{
		0.0f,
		TOOL::RandF() * D3DX_PI * 0.025f ,
		0.0f
	};
	m_vel = Float3(
		TOOL::RandF() * (10.0f / 60.0f) - (5.0f / 60.0f),
		TOOL::RandF() * 10.0f / 60.0f,
		TOOL::RandF() * (10.0f / 60.0f) - (5.0f / 60.0f)
	);

	D3DXMatrixIdentity(&m_World);
}

void ParticleTest::Uninit()
{
}

void ParticleTest::Update()
{
	time++;
	if (time > live_time)
	{
		SetDestroy();
	}

	m_pos += m_vel;

	m_vel.x *= 0.98f;
	m_vel.y *= 0.95f;
	m_vel.z *= 0.98f;

	if (m_pos.y < 0.05f)
		m_vel.y *= -0.96f;
	else
	{
		m_rot.x += m_ang_vel.x;
		m_rot.y += m_ang_vel.y;
		m_rot.z += m_ang_vel.z;
		m_vel.y -= Gravity;
	}

	int testt = live_time - time;
	float testsize = (float)testt / (float)live_time;
	m_scl = DefaultSize * testsize;
}

void ParticleTest::Draw()
{
	//マトリクス設定
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y + m_addrot.y, m_rot.x + m_addrot.x, m_rot.z + m_addrot.z);
	D3DXMatrixTranslation(&trans, m_pos.x + m_addpos.x, m_pos.y + m_addpos.y, m_pos.z + m_addpos.z);

	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);
}

void ParticleTest::Set(D3DXVECTOR3 pos,
	float inGravity,
	int inLiveTime,
	Float3 vel,
	Float3 scl,
	Float3 Rot)
{
	live_time = inLiveTime;
	Gravity = inGravity;
	m_pos = pos;
	m_pos.y += 0.05f;
	m_scl = scl;

	m_vel = Float3(
		TOOL::RandF() * vel.x - (vel.x * 0.5f),
		TOOL::RandF() * vel.y,
		TOOL::RandF() * vel.z - (vel.z * 0.5f));

	m_ang_vel = Float3
	(
		TOOL::RandF() * D3DX_PI * 0.025f * Rot.x,
		TOOL::RandF() * D3DX_PI * 0.025f * Rot.y,
		TOOL::RandF() * D3DX_PI * 0.025f * Rot.z
	);
	DefaultSize = m_scl;
}

void ParticleTest::InstanceDraw()
{
	//マトリクス設定
	D3DXMATRIX scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	m_World = scl * rot * trans;
}