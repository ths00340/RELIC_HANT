//==============================================================================
// Filename: Player.h
// Description :GameObject�N���X���p�������v���C���[�p�N���X�̒�`
//==============================================================================
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
	Model* m_model;//���f���|�C���^

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_VertexLayout;
	ID3D11BlendState  * blendState;

	//���ˉ��p�I�[�f�B�I�|�C���^���R���|�[�l���g�Ɉړ������邽�ߍ폜�\��
	class Audio* m_pShotSE = nullptr;

	//�X�e�[�^�X�R���|�[�l���g�|�C���^
	Status* m_pSta = nullptr;

	//����R���|�[�l���g�|�C���^
	Wepon* m_pWepon = nullptr;

	//�ړ��p(����)�R���|�[�l���g�|�C���^
	DriveSystem* m_pDriveS = nullptr;

	//�ړ��p(������)�R���|�[�l���g�|�C���^
	Drive* m_pDrive = nullptr;

	//HP�\���pUI�I�u�W�F�N�g�|�C���^
	NumberManager* m_pHp = nullptr;

	//MP�\���pUI�I�u�W�F�N�g�|�C���^
	NumberManager* m_pMp = nullptr;

	class Camera* m_pCam = nullptr;

	class Scene* m_pScene = nullptr;

	class cInputOperation* m_pInput;

	//�ԑׂ̂̍��ȐU���p�ϐ�
	float m_Idol = 0.0f;
public:
	//�R���X�g���N�^�[
	Player()
	{
		m_pShotSE = nullptr;
		m_model = nullptr;
		m_pSta = nullptr;
		m_pWepon = nullptr;
		m_pDriveS = nullptr;
		m_pDrive = nullptr;
		m_pHp = nullptr;
		m_pMp = nullptr;
		m_Idol = 0.0f;

		m_VertexShader	= nullptr;
		m_PixelShader	= nullptr;
		m_VertexLayout	= nullptr;
		blendState		= nullptr;
		m_pInput		= nullptr;
	}

	void Init();//������
	void Uninit();//�㏈���p�֐�
	void Update();//�X�V����
	void FixedUpdate();//�S�̂̍X�V��̍X�V����
	void Draw();//�`��

	//�u�����h�X�e�[�g�̓ǂݎ��
	ID3D11BlendState* GetBlendState()override { return blendState; }

	//����R���|�[�l���g�̒ǉ�
	template <typename T>
	void SetWepon()
	{
		DeleteWepon();
		m_pWepon = AddComponent<T>();
	}

	//�ړ��p�R���|�[�l���g(����)�̐ݒ�
	template <typename T>
	void SetDriveS()
	{
		DeleteDriveS();
		m_pDriveS = AddComponent<T>();
	}

	//�ړ��p�R���|�[�l���g(������)�̐ݒ�
	template <typename T>
	void SetDrive()
	{
		DeleteDrive();
		m_pDrive = AddComponent<T>();
	}

	//����R���|�[�l���g�̍폜
	void DeleteWepon()
	{
		if (m_pWepon != nullptr)
		{
			Component.remove(m_pWepon);
			delete m_pWepon;
			m_pWepon = nullptr;
		}
	}

	//�ړ��p�R���|�[�l���g(����)�̍폜
	void DeleteDriveS()
	{
		if (m_pDriveS != nullptr)
		{
			Component.remove(m_pDriveS);
			delete m_pDriveS;
			m_pDriveS = nullptr;
		}
	}

	//�ړ��p�R���|�[�l���g(������)�̍폜
	void DeleteDrive()
	{
		if (m_pDrive != nullptr)
		{
			Component.remove(m_pDrive);
			delete m_pDrive;
			m_pDrive = nullptr;
		}
	}

	void SetHP(int Inhp = 100);//HP�\���pUI�̐ݒ�

	void DeleteHP();//HP�\���pUI�̍폜

	void SetMP();//MP�\���pUI�̐ݒ�

	void Damage(int dmg);//��_���[�W���o�ƌv�Z

	void Finish();//���Ŏ��G�t�F�N�g
};
