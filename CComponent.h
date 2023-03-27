//==============================================================================
// Filename: CComponent.h
// Description: �I�u�W�F�N�g�ɕt�^�����R���|�[�l���g�N���X�̒�`
//==============================================================================

#pragma once
class GameObject;

class CComponent
{
protected:
	GameObject* object;//�t�^���ꂽ�I�u�W�F�N�g
public:
	CComponent(GameObject* gameobj)
	{
		object = gameobj;
	}
	~CComponent()
	{
		object = nullptr;
	}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() {};
	void Setobj(GameObject* obj) { object = obj; }
};
