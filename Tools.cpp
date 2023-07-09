#define _CRT_SECURE_NO_WARNINGS
#include "Tools.h"
#include <stdarg.h>
#include <stdio.h>

const Float3 TOOL::movement(Float3 rot, float spd)
{
	return 	GetForward(rot) * spd;
}

const float TOOL::AToR(float angle)
{
	float i = fabsf(angle);
	return i * (float)D3DX_PI / 180.0f;
}

const int TOOL::RToA(float angle)
{
	float i = fabsf(angle);
	return (180 / D3DX_PI) * i;
}

const Float3 TOOL::QuaternionToEuler(FloatQ inQua)
{
	Float3 ret;

	float q0q0 = inQua.x * inQua.x;
	float q0q1 = inQua.x * inQua.y;
	float q0q2 = inQua.x * inQua.z;
	float q0q3 = inQua.x * inQua.w;
	float q1q1 = inQua.y * inQua.y;
	float q1q2 = inQua.y * inQua.z;
	float q1q3 = inQua.y * inQua.w;
	float q2q2 = inQua.z * inQua.z;
	float q2q3 = inQua.z * inQua.w;
	float q3q3 = inQua.w * inQua.w;
	ret.z = atan2(2.0 * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
	ret.x = asin(2.0 * (q0q2 - q1q3));
	ret.y = atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);

	return ret;
}

const float TOOL::RandF()
{
	return (float)rand() / (float)RAND_MAX;
}

const float TOOL::RandF(float in)
{
	return RandF() * in;
}

const float TOOL::SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
	// タイムステップの制限
	smoothTime = fmaxf(0.0001f, smoothTime);

	// 移動の補間係数を計算
	float omega = 2.0f / smoothTime;
	float x = omega * deltaTime;
	float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

	// 目標値への差を計算
	float diff = current - target;
	float maxDelta = maxSpeed * smoothTime;

	// 速度の補間
	float temp = (currentVelocity + omega * diff) * deltaTime;
	float velocity = Limit(temp, maxDelta, -maxDelta);
	currentVelocity = (currentVelocity - omega * velocity) * exp;

	// 新しい現在値を計算
	float newValue = target + (diff + velocity) * exp;

	return newValue;
}

const bool TOOL::CanRange(Float3 pos1, Float3 pos2, float range)
{
	float x1 = pos1.x - pos2.x;
	float y1 = pos1.y - pos2.y;
	float z1 = pos1.z - pos2.z;

	float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
	a = sqrtf(a);

	return a > range;
}

const bool TOOL::CanHit(Float3 pos1, Float3 pos2, float range)
{
	float x1 = pos1.x - pos2.x;
	float y1 = pos1.y - pos2.y;
	float z1 = pos1.z - pos2.z;

	float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
	a = sqrtf(a);

	return a < range;
}

const Float3 TOOL::GetUp(Float3 inrot)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

	Float3 up;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;

	return up;
}

const Float3 TOOL::GetSide(Float3 inrot)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

	Float3 forward;
	forward.x = rot._11;
	forward.y = rot._12;
	forward.z = rot._13;

	return forward;
}

const Float3 TOOL::GetForward(Float3 inrot)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, inrot.y, inrot.x, inrot.z);

	Float3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;

	return forward;
}
const Float3 TOOL::GetUp(FloatQ inQ)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &inQ);

	Float3 Up;
	Up.x = rot._21;
	Up.y = rot._22;
	Up.z = rot._23;

	return Up;
}
const Float3 TOOL::GetSide(FloatQ inQ)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &inQ);

	Float3 Side;
	Side.x = rot._11;
	Side.y = rot._12;
	Side.z = rot._13;

	return Side;
}
const Float3 TOOL::GetForward(FloatQ inQ)
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &inQ);

	Float3 Forward;
	Forward.x = rot._31;
	Forward.y = rot._32;
	Forward.z = rot._33;

	return Forward;
}

const Float3 TOOL::Float3_Larp(Float3 start, Float3 end, float t)
{
	Float3 ret;
	D3DXVec3Lerp(&ret, &start, &end, t);
	return ret;
}

const float TOOL::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

const float TOOL::PointRange(Float3 infP, Float3 ineP)
{
	float x1 = ineP.x - infP.x;
	float y1 = ineP.y - infP.y;
	float z1 = ineP.z - infP.z;

	float a = (x1 * x1) + (y1 * y1) + (z1 * z1);
	a = sqrtf(a);

	return a;
}

const Float3 TOOL::VectorNormalize(Float3 inVec)
{
	Float3 ret;
	D3DXVec3Normalize(&ret, &inVec);
	return ret;
}

const Float2 TOOL::VectorNormalize(Float2 inVec)
{
	Float2 ret;
	D3DXVec2Normalize(&ret, &inVec);
	return ret;
}

const float TOOL::SubAngle(Float3 Dir, Float3 Dir1)
{
	Float3 A = VectorNormalize(Dir1);
	Float3 NormalDir = VectorNormalize(Dir);

	float vec1 = (NormalDir.x * A.x) + (NormalDir.z * A.z);
	float vec2 = (NormalDir.x * A.z) - (NormalDir.z * A.x);

	return atan2f(vec2, vec1);
}

const float TOOL::SubAngle(Float2 Dir, Float2 Dir1)
{
	Float2 A = VectorNormalize(Dir1);
	Float2 NormalDir = VectorNormalize(Dir);

	float vec1 = (NormalDir.x * A.x) + (NormalDir.y * A.y);
	float vec2 = (NormalDir.x * A.y) - (NormalDir.y * A.x);

	return atan2f(vec2, vec1);
}

const float TOOL::SubAnglex(Float3 Dir, Float3 Dir1)
{
	Float3 A = VectorNormalize(Dir1);
	Float3 NormalDir = VectorNormalize(Dir);

	float mol1, mol2, den;
	mol1 = fabsf(Dir.x) + fabsf(Dir.y) + fabsf(Dir.z);
	mol2 = fabsf(Dir1.x) + fabsf(Dir1.y) + fabsf(Dir1.z);
	den = (Dir.x * Dir1.x) + (Dir.y * Dir1.y) + (Dir.z * Dir1.z);

	return Fraction(den, mol1 * mol2);
}

const float TOOL::SecDiv(float num)
{
	return num * 0.0166f;
}

const float TOOL::Fraction(float deno, float mole)
{
	return mole / deno;
}

const int TOOL::FrameMulti(float num)
{
	return num * 60;
}

std::vector<GameObject*> TOOL::WithinObj(Float3 center, float range, std::vector<GameObject*> CovList)
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

const bool TOOL::HitRaySphere(Float3 Start, Float3 Vel, Float3 CovPos, float r, Float3& HitPos)
{
	CovPos.x = CovPos.x - Start.x;
	CovPos.y = CovPos.y - Start.y;
	CovPos.z = CovPos.z - Start.z;

	float A = Vel.x * Vel.x + Vel.y * Vel.y + Vel.z * Vel.z;
	float B = Vel.x * CovPos.x + Vel.y * CovPos.y + Vel.z * CovPos.z;
	float C = CovPos.x * CovPos.x + CovPos.y * CovPos.y + CovPos.z * CovPos.z - r * r;

	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	HitPos.x = Start.x + a1 * Vel.x;
	HitPos.y = Start.y + a1 * Vel.y;
	HitPos.z = Start.z + a1 * Vel.z;

	return true;
}

std::vector<GameObject*> TOOL::LineObj(Float3 Start, Float3 Rot, std::vector<GameObject*> CovList)
{
	std::vector<GameObject*> objects;
	Float3 Garbage;//ゴミ箱

	for (GameObject* obj : CovList)
	{
		if (HitRaySphere(Start, Rot, obj->Getpos(), obj->Getmax().z * obj->Getscl().z, Garbage))
		{
			objects.push_back(obj);
			Display((char*)"命中(X:%f Y:%f Z:%f)\n", Garbage.x, Garbage.y, Garbage.z);
		}
	}

	return objects;
}

const float TOOL::VectorLength(Float3 inVec)
{
	return sqrtf((inVec.x * inVec.x) + (inVec.y * inVec.y) + (inVec.z * inVec.z));
}

const float TOOL::Magni_one(float size, float aftersize)
{
	float ret = aftersize / size;
	return ret;
}

const Float3 TOOL::Magni_uni(Float3 size, Float3 aftersize)
{
	Float3 ret;
	ret.x = aftersize.x / size.x;
	ret.y = aftersize.y / size.y;
	ret.z = aftersize.z / size.z;
	return ret;
}

const float TOOL::Limit(float in, float max, float min)
{
	float ret = in;
	ret = fminf(max, ret);
	ret = fmaxf(min, ret);
	return ret;
}

const Float3 TOOL::Uniform(float in)
{
	return Float3(in, in, in);
}

const FloatQ TOOL::UniformQ(float in)
{
	return FloatQ(in, in, in, 1.f);
}

const Float3 TOOL::LookatX(Float3 me, Float3 target)
{
	Float3 ret = Uniform();

	//y
	float x1 = me.x - target.x;
	float y1 = me.z - target.z;

	float rot = atan2f(x1, y1);
	ret.y = rot;

	// X回転
	x1 = me.x - target.x;
	float	z1 = me.z - target.z;
	y1 = me.y - target.y;

	rot = atan2f(y1, hypotf(z1, x1));
	ret.x = -rot;

	return ret;
}

const FloatQ TOOL::AddLocalRotation(FloatQ inRot, Float3 inadd)
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

const FloatQ TOOL::AddLocalRotation(FloatQ inRot, float X, float Y, float Z)
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

const FloatQ TOOL::ConvertQ(Float3 inRot)
{
	FloatQ ret;
	D3DXQuaternionRotationYawPitchRoll(&ret, inRot.y, inRot.x, inRot.z);
	return ret;
}

const FloatQ TOOL::ConvertQ(float X, float Y, float Z)
{
	FloatQ ret;
	D3DXQuaternionRotationYawPitchRoll(&ret, Y, X, Z);
	return ret;
}

const Float3 TOOL::CrossProduct(Float3 v1, Float3 v2)
{
	return Float3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

const float TOOL::fBmNoise(const Float2 st, const int octave)
{
	float val = 0.0;
	float a = 0.5;
	Float2 St = st;

	for (int i = 0; i < octave; i++) {
		val += a * noise2(St);
		St *= 2.0f;
		a *= 0.5f;
	}
	return val;
}

const float TOOL::BlurBox(const Float2 st, Float2 midpos, Float2 size, float softness)
{
	Float2 uvOffset = Float2(fabsf(st.x - midpos.x), fabsf(st.y - midpos.y)) - size * 0.5f;
	Float2 clampedOffset = Float2(Limit(uvOffset.x / softness), Limit(uvOffset.y / softness));
	float ret = 1.0f - fmaxf(clampedOffset.x, clampedOffset.y);
	return ret;
}

void TOOL::Display(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char mojiretus[256];
	ZeroMemory(mojiretus, sizeof(mojiretus));
	vsprintf(mojiretus, format, ap);
	OutputDebugString(mojiretus);
}

const float TOOL::noise2(Float2 st)
{
	Float2 p = Float2(floorf(st.x), floorf(st.y));
	Float2 f = Float2(fracf(st.x), fracf(st.y));

	float w00 = dot(rand2(p), f);
	float w10 = dot(rand2(p + Float2(1.0f, 0.0f)), f - Float2(1.0f, 0.0f));
	float w01 = dot(rand2(p + Float2(0.0f, 1.0f)), f - Float2(0.0f, 1.0f));
	float w11 = dot(rand2(p + Float2(1.0f, 1.0f)), f - Float2(1.0f, 1.0f));

	Float2 u = Multiply(Multiply(f, f), (Float2(3.f, 3.f) - (2.0f * f)));

	return lerp(lerp(w00, w10, u.x), lerp(w01, w11, u.x), u.y);
}

const float TOOL::fracf(float value)
{
	return value - floorf(value);
}

const Float2 TOOL::Multiply(const Float2 a, const Float2 b)
{
	Float2 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	return result;
}

const float TOOL::dot(Float2 a, Float2 b)
{
	return a.x * b.x + a.y * b.y;
}

const float TOOL::lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

const float TOOL::step(float a, float x)
{
	return (float)(x >= a) ? 1.f : 0.f;
}

const Float2 TOOL::rand2(Float2 uv)
{
	Float2 st = Float2(dot(uv, Float2(127.1f, 311.7f)),
		dot(uv, Float2(269.5f, 183.3f)));

	return Float2(-1.f, -1.f) + (2.0f * Float2(fracf(sinf(st.x) * 43758.5453123f), fracf(sinf(st.y) * 43758.5453123f)));
}

const float TOOL::Frand(Float2 uv)
{
	return fracf(sin(dot(uv, Float2(12.9898f, 78.233f))) * 43758.5453f);
}

const Float2 TOOL::rand2(int num)
{
	Float2 uv;
	uv.x = (float)(num % 50);
	uv.y = (float)(num / 50);

	Float2 st = Float2(dot(uv, Float2(127.1f, 311.7f)),
		dot(uv, Float2(269.5f, 183.3f)));

	Float2 ret = Float2(-1.f, -1.f) + (2.0f * Float2(fracf(sinf(st.x) * 43758.5453123f), fracf(sinf(st.y) * 43758.5453123f)));
	return Float2(fabsf(ret.x), fabsf(ret.y));
}