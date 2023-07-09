//==============================================================================
// Filename: Tool.h
// Description: �֗��֐����܂Ƃ߂��N���X�̒�`
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//�s�K�v�Ȍx���΍�
#include "main.h"
#include "GameObject.h"
#include <math.h>
#include <list>
#include <vector>

#define GRAVITY (0.3f/60.0f)

enum class CamMode {
	FPP,
	TPP,
	FREE,
};

typedef D3DXVECTOR3 Float3;

class TOOL
{
public:
	//�p�x����Forward���擾����spd��������
	static const Float3 movement(Float3 rot, float spd);//�����Ă�������Ɍ������Đi��

	//�f�B�O���[�����W�A����
	static const float AToR(float angle);//�p�x�����W�A���ɕϊ�

	//���W�A�����f�B�O���[��
	static const int RToA(float angle);

	//�N�H�[�^�j�I�����I�C���[��
	static const Float3 QuaternionToEuler(FloatQ inQua);

	//0-1.0f�̊ԂŋA���Ă��闐��
	static const float RandF();

	//0����in�̒l�ŕԋp
	static const float RandF(float in);

	//�Ǐ]
	static const float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

	//��苗���ȏ�
	static const bool CanRange(Float3 pos1, Float3 pos2, float range);

	//��苗���ȓ�
	static const bool CanHit(Float3 pos1, Float3 pos2, float range);

	//�p�x�����̎擾:�I�C���[�p
	static const Float3 GetUp(Float3 inrot);//��̎擾

	//�p�x���牡�̎擾:�I�C���[�p
	static const Float3 GetSide(Float3 inrot);

	//�p�x���琳�ʂ̎擾:�I�C���[�p
	static const Float3 GetForward(Float3 inrot);

	//�p�x�����̎擾:�N�H�[�^�j�I��
	static const Float3 GetUp(FloatQ inQ);

	//�p�x���牡�̎擾:�N�H�[�^�j�I��
	static const Float3 GetSide(FloatQ inQ);

	//�p�x���琳�ʂ̎擾:�N�H�[�^�j�I��
	static const Float3 GetForward(FloatQ inQ);

	//1���x�W�G�Ȑ��i�����H�j
	static const Float3 Float3_Larp(Float3 start, Float3 end, float t);

	//���`�⊮
	static const float Lerp(float start, float end, float t);

	//�w�肵��2�_�̒�������
	static const float PointRange(Float3 infP = Float3(0.0f, 0.0f, 0.0f), Float3 ineP = Float3(0.0f, 0.0f, 0.0f));

	//�x�N�g���̐��K��
	static const Float3 VectorNormalize(Float3 inVec);

	//�x�N�g���̐��K��
	static const Float2 VectorNormalize(Float2 inVec);

	//�Ȃ��p�@XZ���ʁ@-180~0~180
	static const float SubAngle(Float3 Dir, Float3 Dir1);

	//�Ȃ��p�@2������@-180~0~180
	static const float SubAngle(Float2 Dir, Float2 Dir1);

	//�Ȃ��p��YZ���� 0~180
	static const float SubAnglex(Float3 Dir, Float3 Dir1);

	//60�t���[���p�ɕϊ�
	static const float SecDiv(float num);

	//deno����mole
	static const float Fraction(float deno = 1.0f, float mole = 1.0f);

	//num���t���[����ɕϊ���num=1=60
	static const int FrameMulti(float num);

	//�Ŋ���T�̎擾
	template <typename T>
	static T* NearestPos(Float3 pPos, std::vector<T*> CovList);

	//pPos�F�Ăяo����̍��W�@Dir�FForward�@MaxLange�F�擾�͈́@CovList�F�팟���҂̃��X�g
	template <typename T>//�ł����ʂɋ߂�T
	static T* FlontNearestPos(Float3 pPos, Float3 Dir, float MaxLange, std::vector<T*> CovList);

	//�n�����Q�[���I�u�W�F�N�g���X�g���͈͓��̃I�u�W�F�N�g
	static std::vector<GameObject*> WithinObj(Float3 center, float range, std::vector<GameObject*> CovList);

	//�n����T���X�g���͈͓��̃I�u�W�F�N�g
	template<typename T>
	static std::vector<T*> WithinTObj(Float3 center, float range, std::vector<T*> CovList);

	//���ΐ��̓����蔻��
	static const bool HitRaySphere(Float3 Start, Float3 Vel, Float3 CovPos, float r, Float3& HitPos);

	//�X�^�[�g����L�т����ɓ��������I�u�W�F�N�g��Ԃ�
	static std::vector<GameObject*> LineObj(Float3 Start, Float3 Rot, std::vector<GameObject*> CovList);

	//�x�N�g���̒����̎擾
	static const float VectorLength(Float3 inVec);

	//size��aftersize�ɂ��邽�߂ɂ͉��{�ɂ���΂����̂�
	static const float Magni_one(float size = 1.0f, float aftersize = 1.0f);

	//size��aftersize�ɂ��邽�߂ɂ͊e�����ɉ��{�ɂ���΂����̂�
	static const Float3 Magni_uni(Float3 size = Float3(1.0f, 1.0f, 1.0f), Float3 aftersize = Float3(1.0f, 1.0f, 1.0f));

	//in��min����max�܂ł̊Ԃɐ�������
	static const float Limit(float in = 0.0f, float max = 1.0f, float min = 0.0f);

	//in��xyz�S�ĂɐU�蕪����
	static const Float3 Uniform(float in = 0.0f);

	//in��xyzw�S�ĂɐU�蕪����
	static const FloatQ UniformQ(float in = 0.0f);

	//�ڕW�ւ�x�����Z�o
	static const Float3 LookatX(Float3 me = Float3(0.f, 0.f, 0.f), Float3 target = Float3(0.f, 0.f, 0.f));

	//inRot��inadd�̊p�x�����Z���遦�Ԃ�l��inRot�ɉ��Z���ꂽ�l
	static const FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), Float3 inadd = Float3(0.f, 0.f, 0.f));

	//inRot��XYZ�̊p�x�����Z���遦�Ԃ�l��inRot�ɉ��Z���ꂽ�l
	static const FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), float X = 0.f, float Y = 0.f, float Z = 0.f);

	//Vector3�^����Quaternion�^��
	static const FloatQ ConvertQ(Float3 inRot);

	//Vector3�^����Quaternion�^��
	static const FloatQ ConvertQ(float X, float Y, float Z);

	//�O��
	static const Float3 CrossProduct(Float3 v1, Float3 v2);

	//�m�C�Y����
	static const float fBmNoise(const Float2 st, const int octave);

	//�O�g�ɍs���ɂ�Ăڂ₯��{�b�N�X
	static const float BlurBox(const Float2 st, Float2 midpos, Float2 size, float softness);

	//�f�o�b�O�o�͗p�Fprintf�Ɠ����g����
	static void Display(const char* format, ...);

	static const Float2 rand2(Float2 uv);

	static const Float2 rand2(int num);

	static const float Frand(Float2 uv);

private:
	static const float noise2(Float2 st);

	static const float fracf(float value);

	static const Float2 Multiply(const Float2 a, const Float2 b);

	static const float dot(Float2 a, Float2 b);

	static const float lerp(float a, float b, float x);

	static const float step(float a, float x);
};

template<typename T>
inline T* TOOL::NearestPos(Float3 pPos, std::vector<T*> CovList)
{
	float Lange = 0.0f;

	float lx1 = 0.0f;
	float ly1 = 0.0f;
	float lz1 = 0.0f;

	float A = 0.0f;

	T* ret = nullptr;

	for (T* Cov : CovList)
	{
		Float3 CovPos = Cov->Getpos();

		lx1 = pPos.x - CovPos.x;
		ly1 = pPos.y - CovPos.y;
		lz1 = pPos.z - CovPos.z;

		A = (lx1 * lx1) + (ly1 * ly1) + (lz1 * lz1);
		A = sqrtf(A);

		if (Lange == 0)
		{
			ret = Cov;
			Lange = A;
		}
		if (Lange > A)
		{
			ret = Cov;
			Lange = A;
		}
	}
	return ret;
}

template<typename T>
inline T* TOOL::FlontNearestPos(Float3 pPos, Float3 Dir, float MaxLange, std::vector<T*> CovList)
{
	float vec1 = 0.0f;
	float vec2 = 0.0f;

	float A = 0.0f;
	float Ang = 0.0f;
	Float3 ppulle;
	Float3 NormalDir = VectorNormalize(Dir);

	T* ret = nullptr;

	for (T* Cov : CovList)
	{
		Float3 CovPos = Cov->Getpos();

		if (CanRange(pPos, CovPos, MaxLange))
			continue;

		ppulle = CovPos - pPos;
		ppulle = VectorNormalize(ppulle);
		vec1 = (Dir.x * ppulle.x) + (Dir.z * ppulle.z);
		vec2 = (Dir.x * ppulle.z) - (Dir.z * ppulle.x);

		A = atan2f(vec2, vec1);

		if (Ang == 0.0f)
		{
			ret = Cov;
			Ang = A;
		}

		if (fabsf(Ang) > fabsf(A))
		{
			ret = Cov;
			Ang = A;
		}
		if (A == 0)
		{
			ret = Cov;
			Ang = A;
		}
	}
	return ret;
}

template<typename T>
inline std::vector<T*> TOOL::WithinTObj(Float3 center, float range, std::vector<T*> CovList)
{
	std::vector<T*> objects;

	for (T* obj : CovList)
	{
		if (CanHit(center, obj->Getpos(), range))
		{
			objects.push_back(obj);
		}
	}

	return objects;
}
