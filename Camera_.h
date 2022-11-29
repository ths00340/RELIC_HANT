#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "manager.h"
#include "Tools.h"
#include "GameObject.h"

enum class MENU
{
	GARAGE,
	STAGE,
	SHOP,
	MAX
};

class Camera_ :
	public GameObject
{
private:
	CamMode Mode;
	Float3 m_pos;
	Float3 m_tag;
	Float3 m_up;
	D3DXMATRIX m_ViewMatrix;
	Float3 Dir;
	Float3 Angle;
	float ViewAng;//Ž‹–ìŠp
	GameObject* tag = NULL;

	int menu = (int)MENU::STAGE;
	Float3 Nextpos[(int)MENU::MAX];
	Float3 Nexttag[(int)MENU::MAX];
	float time;
public:
	Camera_()
	{
		m_pos = Float3(0.0f, 3.0f, 10.0f);
		m_tag = Float3(0.0f, 0.0f, 0.0f);
		m_up = Float3(0.0f, 1.0f, 0.0f);
		Dir = Float3(0.0f, 0.0f, 0.0f);
		Angle = Float3(0.0f, 0.0f, 0.0f);
		ViewAng = 1.0f;
		Mode = CamMode::TPP;
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetTag(D3DXVECTOR3 intag) { m_tag = intag; };
	void SetUp(D3DXVECTOR3 inup) { m_up = inup; };
	void SetPos(Float3 inpos) { m_pos = inpos; };
	void SetMode(CamMode InMode)
	{
		Mode = InMode;
	};
	void Addmenu(int in);

	int Getmenu() { return menu; }

	GameObject* GetTarget() { return tag; }
	Float3 GetDir();
	Float3 GetAngle() { return Angle; };
	Float3 GetTag() { return m_tag; }
	CamMode GetMode() { return Mode; }
	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};
