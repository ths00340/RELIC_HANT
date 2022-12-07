//==============================================================================
// Filename: HitBox.h
// Description: コンポーネントクラスを継承した当たり判定クラスの定義
//==============================================================================
#pragma once
#include "CComponent.h"

enum class HITBOX_TYPE
{
	CUBE,//四角
	SPHERE,//円
	TYPE_END,
};

class HitBox :
	public CComponent
{
	static Model* model[(int)HITBOX_TYPE::TYPE_END];

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertexLayout;
	static ID3D11BlendState* blendState;

	Float3 m_size;
	Float3 addpos;
	HITBOX_TYPE type;
	bool draw = true;
public:
	HitBox(GameObject* inobject) :CComponent(inobject)
	{
		m_size = Float3(0.0f, 0.0f, 0.0f);
		addpos = Float3(0.0f, 0.0f, 0.0f);
		draw = false;
		type = HITBOX_TYPE::SPHERE;
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Set(HITBOX_TYPE intype = HITBOX_TYPE::CUBE, bool indraw = false);
	HITBOX_TYPE Gettype() { return type; }
	const bool GetDraw() { return draw; }

	static void Load();
};
