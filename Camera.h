//==============================================================================
// Filename: Camera.h
// Description: コンポーネントクラスを継承したカメラクラスの定義
//==============================================================================
#pragma once
#include <typeinfo>
#include "manager.h"
#include "Tools.h"
#include "CComponent.h"

class GameObject;

class Camera :public CComponent
{
private:
	CamMode Mode;//FPPかTPPか
	Float3 m_pos;//座標
	Float3 m_tag;//注視点
	Float3 m_up;//頂点
	D3DXMATRIX m_ViewMatrix;//視点行列
	Float3 Dir;//座標から注視点へのベクトル
	Float3 Angle;//角度
	Float3 Addpos;//追加座標
	float ViewAng;//視野角
	GameObject* tag = NULL;//ターゲットオブジェクト
	float Range;//取得範囲
	class cInputOperation* m_pInput;

	/// 揺れ用変数
	float  time = 0.f;

	bool shakeP = false;
	bool shakeR = false;

	float MaxRandP = 0.f;
	float MaxRandR = 0.f;
	float shakeStrP = 1.f;
	float shakeStrR = 1.f;

public:

	Camera(GameObject* inobject) :CComponent(inobject) {
		m_pos = Float3(0.0f, 3.0f, -5.0f);
		m_tag = TOOL::Uniform();
		m_up = Float3(0.0f, 1.0f, 0.0f);
		Dir = TOOL::Uniform();
		Angle = TOOL::Uniform();
		Addpos = TOOL::Uniform();
		ViewAng = 1.0f;
		Range = 30.0f;
		Mode = CamMode::TPP;
		shakeP = false;
		shakeR = false;
		time = 0;
		MaxRandP = 0;
		MaxRandR = 0;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//一番画面中央に近い敵をターゲットする
	template <typename T>
	T* FlontTarget() {
		if (Mode == CamMode::FPP)
			return NULL;

		T* rettag = NULL;
		Scene* scene = Manager::GetScene();

		//ターゲッティング
		if (!tag)
		{
			if (TOOL::FlontNearestPos<T>(object->Getpos(), Dir, Range, scene->GetGameObjects<T>()))
			{
				rettag = TOOL::FlontNearestPos<T>(object->Getpos(), Dir, Range, scene->GetGameObjects<T>());
				tag = rettag;
			}
		}
		else
		{
			tag = NULL;
		}
		return rettag;
	}

	void SetRange(float inRange) { Range = inRange; }
	void SetTag(D3DXVECTOR3 intag);
	void SetUp(D3DXVECTOR3 inup);
	void SetPos(Float3 inpos) { m_pos = inpos; };
	void SetMode(CamMode InMode)
	{
		Mode = InMode;
	};
	void SetTarget(GameObject* intag)
	{
		if (tag == NULL)
			tag = intag;
	}
	void SetShakePos(float max = 1.f, float Str = 1.f);
	void SetShakeRot(float max = 1.f, float Str = 1.f);

	const Float3 GetDir();
	const Float3 GetAngle() { return Angle; };
	const Float3 GetTag() { return m_tag; }
	const Float3 GetUp() { return m_up; }
	const Float3 GetPos() { return m_pos; }
	const CamMode GetMode() { return Mode; }
	const float GetViewAng() { return ViewAng; }
	const D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

	//ポインタ渡し
	Float3* GetTagPoint() { return &m_tag; }
	Float3* GetUpPoint() { return &m_up; }
	Float3* GetPosPoint() { return &m_pos; }
	D3DXMATRIX* GetViewMatrixPoint() { return &m_ViewMatrix; }
	GameObject* GetTarget() { return tag; }
};
