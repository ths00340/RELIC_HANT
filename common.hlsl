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

cbuffer CameraBuffer : register(b5) //�o�b�t�@�̂T�ԂƂ���
{
	float4 CameraPosition; //�J�������W���󂯂Ƃ�ϐ�
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

///�V�F�[�_�[�p�̎���
/*����
_SinTime.x->w/8
_SinTime.y->w/4
_SinTime.z->w/2
_SinTime.w->���ۂ̎���
*/
cbuffer TimeBuffer : register(b7)
{
	float4 _SinTime;
}

//����
float rand(float2 co) //�����̓V�[�h�l�ƌĂ΂��@�����l��n���Γ������̂�Ԃ�
{
	return frac(sin(dot(co.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}

//�p�[�����m�C�Y
float perlinNoise(float2 st)
{

	float2 p = floor(st);
	float2 f = frac(st);
	float2 u = f * f * (3.f - 2.f * f);

	float v00 = rand(p + float2(0.f, 0.f));
	float v10 = rand(p + float2(1.f, 0.f));
	float v01 = rand(p + float2(0.f, 1.f));
	float v11 = rand(p + float2(1.f, 1.f));

	return lerp(lerp(dot(v00, f - float2(0.f, 0.f)), dot(v10, f - float2(1.f, 0.f)), u.x),
		lerp(dot(v01, f - float2(0.f, 1.f)), dot(v11, f - float2(1.f, 1.f)), u.x)
		, u.y) + 0.5f;
}

float fBm(float2 st)
{
	float f = 0;
	float2 q = st;

	f += 0.5000 * perlinNoise(q); q = q * 2.01;
	f += 0.2500 * perlinNoise(q); q = q * 2.02;
	f += 0.1250 * perlinNoise(q); q = q * 2.03;
	f += 0.0625 * perlinNoise(q); q = q * 2.01;

	return f;
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

	matrix ViewMatrix; //�ǉ� ���C�g�J�����s��
	matrix ProjectionMatrix; //�ǉ� ���C�g�v���W�F�N�V�����s��
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
	float4 ShadowPosition : POSITION1; //���C�g�J�������猩���s�N�Z���̍��W
};