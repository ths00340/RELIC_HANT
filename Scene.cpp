#include "Scene.h"

#include "bullet.h"
#include "Enemy.h"
#include "ParticleManager.h"
#include "ParticleTest.h"
#include "Skybox.h"
#include "Explosion.h"
#include "Player.h"
#include "Polygon2D.h"
#include "Tree.h"
#include "Platform.h"
#include "Leg_01.h"
#include "SphereShadow.h"
#include "Bazooka.h"
#include "ExplosiveBullet.h"
#include "HitBox.h"
#include "CarWheel.h"
#include "ExplodeDome.h"
#include "ChargeLaser.h"
#include "LaserBullet.h"
#include "ChargeEffect.h"
#include "Gatling.h"
#include "ShutterFade.h"
#include "NormalFade.h"
#include "Fissure.h"
#include "ShotGun_Physics.h"

bool Scene::isLoad;

void Scene::Update()
{
	for (int i = 0; i < LAYER_NUM; i++)
	{
		for (GameObject* object : g_GameObject[i])//”ÍˆÍforƒ‹[ƒv
		{
			if (object->GetStop())
				continue;

			object->Update();

			for (CComponent* com : object->GetComponent())
				com->Update();

			object->FixedUpdate();
		}
		g_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
	}
}

void Scene::Loads()
{
	Bullet::Load();
	ParticleManager::Load();
	Player::Load();
	Enemy::Load();
	Skybox::Load();
	Explosion::Load();
	Tree::Load();
	Platform::Load();
	Leg_01::Load();
	SphereShadow::Load();
	Bazooka::Load();
	ExplosiveBullet::Load();
	HitBox::Load();
	CarWheel::Load();
	ExplodeDome::Load();
	ChargeLaser::Load();
	LaserBullet::Load();
	ChargeEffect::Load();
	Gatling::Load();
	ShutterFade::Load();
	NormalFade::Load();
	Fissure::Load();
	ShotGun_Physics::Load();

	isLoad = true;
}

void Scene::UnLoads()
{
	Explosion::UnLoad();
	Tree::UnLoad();
	SphereShadow::UnLoad();
	ChargeEffect::UnLoad();
	ShutterFade::UnLoad();
	NormalFade::UnLoad();
	Fissure::UnLoad();
}