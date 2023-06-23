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
	static Float3 movement(Float3 rot, float spd)//�����Ă�������Ɍ������Đi��
	{
		return 	GetForward(rot) * spd;
	}

	//�f�B�O���[�����W�A����
	static float AToR(float angle)//�p�x�����W�A���ɕϊ�
	{
		float i = fabsf(angle);
		return i * (float)D3DX_PI / 180.0f;
	}

	//���W�A�����f�B�O���[��
	static int RToA(float angle)//�p�x�����W�A���ɕϊ�
	{
		float i = fabsf(angle);
		return (180 / D3DX_PI) * i;
	}

	//0-1.0f�̊ԂŋA���Ă��闐��
	static float RandF()
	{
		return (float)rand() / (float)RAND_MAX;
	}

	static float RandF(float in)
	{
		return RandF() * in;
	}

	//��苗���ȏ�
	static bool CanRange(Float3 pos1, Float3 pos2, float range)
	{
		float x1 = pos1.x - pos2.x;
		float y1 = pos1.y - pos2.y;
		float z1 = pos1.z - pos2.z;

		float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
		a = sqrtf(a);

		return a > range;
	}

	//��苗���ȓ�
	static bool CanHit(Float3 pos1, Float3 pos2, float range)
	{
		float x1 = pos1.x - pos2.x;
		float y1 = pos1.y - pos2.y;
		float z1 = pos1.z - pos2.z;

		float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
		a = sqrtf(a);

		return a < range;
	}

	//�p�x�����̎擾:�I�C���[�p
	static Float3 GetUp(Float3 inrot)//��̎擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

		Float3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	//�p�x���牡�̎擾:�I�C���[�p
	static Float3 GetSide(Float3 inrot)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

		Float3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	//�p�x���琳�ʂ̎擾:�I�C���[�p
	static Float3 GetForward(Float3 inrot)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

		Float3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	//�p�x�����̎擾:�N�H�[�^�j�I��
	static Float3 GetUp(FloatQ inQ)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &inQ);

		Float3 Up;
		Up.x = rot._21;
		Up.y = rot._22;
		Up.z = rot._23;

		return Up;
	}

	//�p�x���牡�̎擾:�N�H�[�^�j�I��
	static Float3 GetSide(FloatQ inQ)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &inQ);

		Float3 Side;
		Side.x = rot._11;
		Side.y = rot._12;
		Side.z = rot._13;

		return Side;
	}

	//�p�x���琳�ʂ̎擾:�N�H�[�^�j�I��
	static Float3 GetForward(FloatQ inQ)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &inQ);

		Float3 Forward;
		Forward.x = rot._31;
		Forward.y = rot._32;
		Forward.z = rot._33;

		return Forward;
	}

	//1���x�W�G�Ȑ��i�����H�j
	static Float3 Float3_Larp(Float3 start, Float3 end, float t)
	{
		Float3 ret;
		D3DXVec3Lerp(&ret, &start, &end, t);
		return ret;
	}

	//�w�肵��2�_�̒�������
	static float PointRange(Float3 infP = Float3(0.0f, 0.0f, 0.0f), Float3 ineP = Float3(0.0f, 0.0f, 0.0f))
	{
		float x1 = ineP.x - infP.x;
		float y1 = ineP.y - infP.y;
		float z1 = ineP.z - infP.z;

		float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
		a = sqrtf(a);

		return a;
	}

	//�x�N�g���̐��K��
	static Float3 VectorNormalize(Float3 inVec)
	{
		Float3 ret;
		D3DXVec3Normalize(&ret, &inVec);
		return ret;
	}

	//�Ȃ��p��XZ���ʁ@-180~0~180
	static float SubAngle(Float3 Dir, Float3 Dir1)
	{
		Float3 A = VectorNormalize(Dir1);
		Float3 NormalDir = VectorNormalize(Dir);

		float vec1 = (NormalDir.x * A.x) + (NormalDir.z * A.z);
		float vec2 = (NormalDir.x * A.z) - (NormalDir.z * A.x);

		return atan2f(vec2, vec1);
	}

	//�Ȃ��p��YZ���� 0~180
	static float SubAnglex(Float3 Dir, Float3 Dir1)
	{
		Float3 A = VectorNormalize(Dir1);
		Float3 NormalDir = VectorNormalize(Dir);

		float mol1, mol2, den;
		mol1 = fabsf(Dir.x) + fabsf(Dir.y) + fabsf(Dir.z);
		mol2 = fabsf(Dir1.x) + fabsf(Dir1.y) + fabsf(Dir1.z);
		den = (Dir.x * Dir1.x) + (Dir.y * Dir1.y) + (Dir.z * Dir1.z);

		return Fraction(den, mol1 * mol2);
	}

	//1�t���[���p�ɕϊ�
	static float SecDiv(float num)
	{
		return num * 0.0166f;
	}

	//deno����mole
	static float Fraction(float deno = 1.0f, float mole = 1.0f)
	{
		return mole / deno;
	}

	//num���t���[����ɕϊ���num=1=60
	static int FrameMulti(float num)
	{
		return num * 60;
	}

	//�Ŋ���T�̎擾
	template <typename T>
	static T* NearestPos(Float3 pPos, std::vector<T*> CovList)
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

	//pPos�F�Ăяo����̍��W�@Dir�FForward�@MaxLange�F�擾�͈́@CovList�F�팟���҂̃��X�g
	template <typename T>//�ł����ʂɋ߂�T
	static T* FlontNearestPos(Float3 pPos, Float3 Dir, float MaxLange, std::vector<T*> CovList)
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

	//�n�����Q�[���I�u�W�F�N�g���X�g���͈͓��̃I�u�W�F�N�g
	static std::vector<GameObject*> WithinObj(Float3 center, float range, std::vector<GameObject*> CovList)
	{
		std::vector<GameObject*> objects;

		for (GameObject* obj : CovList)
		{
			if (CanHit(center, obj->Getpos(), range))
			{
				objects.push_back(obj);
			}
		}

		return objects;
	}

	//�n����T���X�g���͈͓��̃I�u�W�F�N�g
	template<typename T>
	static std::vector<T*> WithinTObj(Float3 center, float range, std::vector<T*> CovList)
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

	//���ΐ��̓����蔻��
	static const bool HitRaySphere(Float3 Start, Float3 Vel, Float3 CovPos, float r, Float3& HitPos)
	{
		CovPos.x = CovPos.x - Start.x;
		CovPos.y = CovPos.y - Start.y;
		CovPos.z = CovPos.z - Start.z;

		float A = Vel.x * Vel.x + Vel.y * Vel.y + Vel.z * Vel.z;
		float B = Vel.x * CovPos.x + Vel.y * CovPos.y + Vel.z * CovPos.z;
		float C = CovPos.x * CovPos.x + CovPos.y * CovPos.y + CovPos.z * CovPos.z - r * r;

		if (A == 0.0f)
			return false; // ���C�̒�����0

		float s = B * B - A * C;
		if (s < 0.0f)
			return false; // �Փ˂��Ă��Ȃ�

		s = sqrtf(s);
		float a1 = (B - s) / A;
		float a2 = (B + s) / A;

		if (a1 < 0.0f || a2 < 0.0f)
			return false; // ���C�̔��΂ŏՓ�

		HitPos.x = Start.x + a1 * Vel.x;
		HitPos.y = Start.y + a1 * Vel.y;
		HitPos.z = Start.z + a1 * Vel.z;

		return true;
	}

	//�X�^�[�g����L�т����ɓ��������I�u�W�F�N�g��Ԃ�
	static std::vector<GameObject*> LineObj(Float3 Start, Float3 Rot, std::vector<GameObject*> CovList)
	{
		std::vector<GameObject*> objects;
		Float3 Garbage;//�S�~��

		for (GameObject* obj : CovList)
		{
			if (HitRaySphere(Start, Rot, obj->Getpos(), obj->Getmax().z * obj->Getscl().z, Garbage))
			{
				objects.push_back(obj);
				Display((char*)"����(X:%f Y:%f Z:%f)\n", Garbage.x, Garbage.y, Garbage.z);
			}
		}

		return objects;
	}

	//�x�N�g���̒����̎擾
	static float VectorLength(Float3 inVec)
	{
		return sqrtf((inVec.x * inVec.x) + (inVec.y * inVec.y) + (inVec.z * inVec.z));
	}

	//size��aftersize�ɂ��邽�߂ɂ͉��{�ɂ���΂����̂�
	static const float Magni_one(float size = 1.0f, float aftersize = 1.0f)
	{
		float ret = aftersize / size;
		return ret;
	}

	//size��aftersize�ɂ��邽�߂ɂ͊e�����ɉ��{�ɂ���΂����̂�
	static const Float3 Magni_uni(Float3 size = Float3(1.0f, 1.0f, 1.0f), Float3 aftersize = Float3(1.0f, 1.0f, 1.0f))
	{
		Float3 ret;
		ret.x = aftersize.x / size.x;
		ret.y = aftersize.y / size.y;
		ret.z = aftersize.z / size.z;
		return ret;
	}

	//in��min����max�܂ł̊Ԃɐ�������
	static const float Limit(float in = 0.0f, float max = 1.0f, float min = 0.0f) {
		float ret = in;
		ret = fminf(max, ret);
		ret = fmaxf(min, ret);
		return ret;
	}

	//in��xyz�S�ĂɐU�蕪����
	static const Float3 Uniform(float in = 0.0f) {
		return Float3(in, in, in);
	}

	//in��xyzw�S�ĂɐU�蕪����
	static const FloatQ UniformQ(float in = 0.0f) {
		return FloatQ(in, in, in, 1.f);
	}

	//�ڕW�ւ�x�����Z�o
	static const Float3 LookatX(Float3 me = Uniform(), Float3 target = Uniform())
	{
		Float3 ret = Uniform();

		//y
		float x1 = me.x - target.x;
		float y1 = me.z - target.z;

		float rot = atan2f(x1, y1);
		ret.y = rot;

		// X��]
		x1 = me.x - target.x;
		float	z1 = me.z - target.z;
		y1 = me.y - target.y;

		rot = atan2f(y1, hypotf(z1, x1));
		ret.x = -rot;

		return ret;
	}

	//inRot��inadd�̊p�x�����Z���遦�Ԃ�l��inRot�ɉ��Z���ꂽ�l
	static FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), Float3 inadd = Float3(0.f, 0.f, 0.f))
	{
		D3DXQUATERNION quat;
		FloatQ ret = inRot;
		Float3 axis = GetUp(ret);

		if (inadd.y != 0.f)
		{
			D3DXQuaternionRotationAxis(&quat, &axis, inadd.y);
			ret *= quat;
		}
		if (inadd.y != 0.f)
		{
			axis = GetSide(ret);
			D3DXQuaternionRotationAxis(&quat, &axis, inadd.x);
			ret *= quat;
		}
		if (inadd.y != 0.f)
		{
			axis = GetForward(ret);
			D3DXQuaternionRotationAxis(&quat, &axis, inadd.z);
			ret *= quat;
		}
		return ret;
	}

	//inRot��XYZ�̊p�x�����Z���遦�Ԃ�l��inRot�ɉ��Z���ꂽ�l
	static FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), float X = 0.f, float Y = 0.f, float Z = 0.f)
	{
		D3DXQUATERNION quat;
		Float3 axis;
		FloatQ ret = inRot;
		if (Y != 0.f)
		{
			axis = GetUp(ret);
			D3DXQuaternionRotationAxis(&quat, &axis, Y);
			ret *= quat;
		}
		if (X != 0.f)
		{
			axis = GetSide(ret);
			D3DXQuaternionRotationAxis(&quat, &axis, X);
			ret *= quat;
		}

		if (Z != 0.f)
		{
			axis = GetForward(ret);
			D3DXQuaternionRotationAxis(&quat, &axis, Z);
			ret *= quat;
		}
		return ret;
	}

	//Vector3�^����Quaternion�^��
	static FloatQ ConvertQ(Float3 inRot)
	{
		FloatQ ret;
		D3DXQuaternionRotationYawPitchRoll(&ret, inRot.y, inRot.x, inRot.z);
		return ret;
	}

	static FloatQ ConvertQ(float X, float Y, float Z)
	{
		FloatQ ret;
		D3DXQuaternionRotationYawPitchRoll(&ret, Y, X, Z);
		return ret;
	}

	static Float3 CrossProduct(const Float3& v1, const Float3& v2)
	{
		return Float3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	//�f�o�b�O�o�͗p�Fprintf�Ɠ����g����
	static void Display(char* format, ...);
};
