cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

cbuffer CameraBuffer : register(b5) //バッファの５番とする
{
	float4 CameraPosition; //カメラ座標を受けとる変数
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}

struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Position;
	float4 Diffuse;
	float4 Ambient;

	matrix ViewMatrix; //追加 ライトカメラ行列
	matrix ProjectionMatrix; //追加 ライトプロジェクション行列
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};

struct PS_IN
{
	float4 Position		:SV_POSITION;
	float4 WorldPosition:POSITION0;
	float4 Normal		:NORMAL0;
	float4 Diffuse		:COLOR0;
	float2 TexCoord		:TEXCOORD0;
	float4 ShadowPosition : POSITION1; //ライトカメラから見たピクセルの座標
};