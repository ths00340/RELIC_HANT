//==============================================================================
// Filename: Tool.h
// Description: 便利関数をまとめたクラスの定義
//==============================================================================
#pragma once
#pragma warning( disable : 4244 )//不必要な警告対策
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
	//角度からForwardを取得してspdをかける
	static const Float3 movement(Float3 rot, float spd);//向いている方向に向かって進む

	//ディグリーをラジアンに
	static const float AToR(float angle);//角度をラジアンに変換

	//ラジアンをディグリーに
	static const int RToA(float angle);

	//クォータニオンをオイラーに
	static const Float3 QuaternionToEuler(FloatQ inQua);

	//0-1.0fの間で帰ってくる乱数
	static const float RandF();

	//0からinの値で返却
	static const float RandF(float in);

	//追従
	static const float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

	//一定距離以上
	static const bool CanRange(Float3 pos1, Float3 pos2, float range);

	//一定距離以内
	static const bool CanHit(Float3 pos1, Float3 pos2, float range);

	//角度から上の取得:オイラー角
	static const Float3 GetUp(Float3 inrot);//上の取得

	//角度から横の取得:オイラー角
	static const Float3 GetSide(Float3 inrot);

	//角度から正面の取得:オイラー角
	static const Float3 GetForward(Float3 inrot);

	//角度から上の取得:クォータニオン
	static const Float3 GetUp(FloatQ inQ);

	//角度から横の取得:クォータニオン
	static const Float3 GetSide(FloatQ inQ);

	//角度から正面の取得:クォータニオン
	static const Float3 GetForward(FloatQ inQ);

	//1次ベジエ曲線（直線？）
	static const Float3 Float3_Larp(Float3 start, Float3 end, float t);

	//線形補完
	static const float Lerp(float start, float end, float t);

	//指定した2点の直線距離
	static const float PointRange(Float3 infP = Float3(0.0f, 0.0f, 0.0f), Float3 ineP = Float3(0.0f, 0.0f, 0.0f));

	//ベクトルの正規化
	static const Float3 VectorNormalize(Float3 inVec);

	//ベクトルの正規化
	static const Float2 VectorNormalize(Float2 inVec);

	//なす角　XZ平面　-180~0~180
	static const float SubAngle(Float3 Dir, Float3 Dir1);

	//なす角　2次元上　-180~0~180
	static const float SubAngle(Float2 Dir, Float2 Dir1);

	//なす角※YZ平面 0~180
	static const float SubAnglex(Float3 Dir, Float3 Dir1);

	//60フレーム用に変換
	static const float SecDiv(float num);

	//deno分のmole
	static const float Fraction(float deno = 1.0f, float mole = 1.0f);

	//numをフレーム基準に変換※num=1=60
	static const int FrameMulti(float num);

	//最寄りのTの取得
	template <typename T>
	static T* NearestPos(Float3 pPos, std::vector<T*> CovList);

	//pPos：呼び出し主の座標　Dir：Forward　MaxLange：取得範囲　CovList：被検索者のリスト
	template <typename T>//最も正面に近いT
	static T* FlontNearestPos(Float3 pPos, Float3 Dir, float MaxLange, std::vector<T*> CovList);

	//渡したゲームオブジェクトリストかつ範囲内のオブジェクト
	static std::vector<GameObject*> WithinObj(Float3 center, float range, std::vector<GameObject*> CovList);

	//渡したTリストかつ範囲内のオブジェクト
	template<typename T>
	static std::vector<T*> WithinTObj(Float3 center, float range, std::vector<T*> CovList);

	//球対線の当たり判定
	static const bool HitRaySphere(Float3 Start, Float3 Vel, Float3 CovPos, float r, Float3& HitPos);

	//スタートから伸びた線に当たったオブジェクトを返す
	static std::vector<GameObject*> LineObj(Float3 Start, Float3 Rot, std::vector<GameObject*> CovList);

	//ベクトルの長さの取得
	static const float VectorLength(Float3 inVec);

	//sizeをaftersizeにするためには何倍にすればいいのか
	static const float Magni_one(float size = 1.0f, float aftersize = 1.0f);

	//sizeをaftersizeにするためには各方向に何倍にすればいいのか
	static const Float3 Magni_uni(Float3 size = Float3(1.0f, 1.0f, 1.0f), Float3 aftersize = Float3(1.0f, 1.0f, 1.0f));

	//inをminからmaxまでの間に制限する
	static const float Limit(float in = 0.0f, float max = 1.0f, float min = 0.0f);

	//inをxyz全てに振り分ける
	static const Float3 Uniform(float in = 0.0f);

	//inをxyzw全てに振り分ける
	static const FloatQ UniformQ(float in = 0.0f);

	//目標へのx軸を算出
	static const Float3 LookatX(Float3 me = Float3(0.f, 0.f, 0.f), Float3 target = Float3(0.f, 0.f, 0.f));

	//inRotにinaddの角度を加算する※返り値はinRotに加算された値
	static const FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), Float3 inadd = Float3(0.f, 0.f, 0.f));

	//inRotにXYZの角度を加算する※返り値はinRotに加算された値
	static const FloatQ AddLocalRotation(FloatQ inRot = FloatQ(0.f, 0.f, 0.f, 1.f), float X = 0.f, float Y = 0.f, float Z = 0.f);

	//Vector3型からQuaternion型へ
	static const FloatQ ConvertQ(Float3 inRot);

	//Vector3型からQuaternion型へ
	static const FloatQ ConvertQ(float X, float Y, float Z);

	//外積
	static const Float3 CrossProduct(Float3 v1, Float3 v2);

	//ノイズ生成
	static const float fBmNoise(const Float2 st, const int octave);

	//外枠に行くにつれてぼやけるボックス
	static const float BlurBox(const Float2 st, Float2 midpos, Float2 size, float softness);

	//デバッグ出力用：printfと同じ使い方
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
