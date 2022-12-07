#pragma once
#include "GameObject.h"
#include "Wepon.h"
#include "DriveSystem.h"
#include "Drive.h"
#define SE_MAX_NUM (10)

class NumberManager;
class Model;

class Player :public GameObject
{
private:
	static Model* m_model;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;

	class Audio* m_ShotSE = nullptr;

	Status* sta = nullptr;
	Wepon* wepon = nullptr;
	DriveSystem* driveS = nullptr;
	Drive* drive = nullptr;
	NumberManager* Hp = nullptr;
	NumberManager* Mp = nullptr;
	float Idol = 0.0f;
public:
	Player()
	{
		m_ShotSE = nullptr;

		sta = nullptr;
		wepon = nullptr;
		driveS = nullptr;
		drive = nullptr;
		Hp = nullptr;
		Mp = nullptr;
		Idol = 0.0f;
	}

	static void Load();
	void Init();
	void Uninit();
	void Update();
	void FixedUpdate();
	void Draw();
	ID3D11BlendState* GetBlendState()override { return blendState; }

	template <typename T>
	void SetWepon()
	{
		DeleteWepon();
		wepon = AddComponent<T>();
	}

	template <typename T>
	void SetDriveS()
	{
		DeleteDriveS();
		driveS = AddComponent<T>();
	}

	template <typename T>
	void SetDrive()
	{
		DeleteDrive();
		drive = AddComponent<T>();
	}

	void DeleteWepon()
	{
		if (wepon != nullptr)
		{
			Component.remove(wepon);
			delete wepon;
			wepon = nullptr;
		}
	}

	void DeleteDriveS()
	{
		if (driveS != nullptr)
		{
			Component.remove(driveS);
			delete driveS;
			driveS = nullptr;
		}
	}

	void DeleteDrive()
	{
		if (drive != nullptr)
		{
			Component.remove(drive);
			delete drive;
			drive = nullptr;
		}
	}

	void SetHP(int Inhp = 100);

	void DeleteHP();

	void SetMP();

	void Damage(int dmg);
	void Finish();
};
