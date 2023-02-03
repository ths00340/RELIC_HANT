struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

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

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
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

struct PS_EdgeInput {
	float4 Position	: SV_POSITION;
	float2 tex0	: TEXCOORD0;
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
	float4 tex8 : TEXCOORD8;
};

struct PS_OUT
{
	float4 Diffuse :SV_Target0;//�ʏ�
	float4 Normal:SV_Target1;//�@��
};