#pragma once
#include "CComponent.h"
class DebugObject :
	public CComponent
{
private:
	class Model* DebugOBJ;//ó\ë™ê¸

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11BlendState* blendState;
	Float3 m_size;

public:
	DebugObject(GameObject* inobject) :CComponent(inobject) {}
	void Init();
	void Update();
	void Uninit();
	void Draw();
};
