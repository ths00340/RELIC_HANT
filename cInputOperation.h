//==============================================================================
// Filename: cInputOperation.h
// Description: �R���|�[�l���g�N���X���p������������͂̒�`
//==============================================================================
#pragma once
#include "CComponent.h"
class cInputOperation :
	public CComponent
{
public:
	cInputOperation(GameObject* inobject) :CComponent(inobject) {}
	void Init();
	void Uninit();
	void Update();
	//���X�e�B�b�N+WASD�̓��͒l
	Float2 GetMov() { return m_Mov; };

	//�E�X�e�B�b�N+�}�E�X
	Float2 GetPov() { return m_Pov; };

	//�ˌ��L�[��������Ă��邩�ۂ�
	bool GetShot();

	//�`��(���b�N�I��)�L�[��������Ă��邩�ۂ�
	bool GetAds();

	//�I�v�V�����L�[��������Ă��邩�ۂ�
	bool GetOption();

	//����L�[��������Ă��邩�ۂ�
	bool GetOK();

	//���ۃL�[��������Ă��邩�ۂ�
	bool GetCancel();

	//�W�����v�L�[��������Ă��邩�ۂ�
	bool GetJump();

private:
	int m_PadNum = 0;//����U��ꂽ�p�b�h
	Float2 m_Pov;//���_
	Float2 m_Mov;//�ړ�
};
