//==============================================================================
// Filename: SphereShadow.h
// Description: コンポーネントクラスを継承した影クラスの定義
//==============================================================================

//地形に対応していないので今は使えない
#pragma once
#include "CComponent.h"
class SphereShadow :
	public CComponent
{
private:
	static ID3D11Buffer* m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	Float3 m_pos;
	Float3 m_rot;
	Float3 m_scl;
public:
	SphereShadow(GameObject* inobject) :CComponent(inobject) {
	}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static void Load();
	static void UnLoad();
};
