//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//
//DirectX�̃e�N�X�`���̐ݒ���󂯌p��
Texture2D g_Texture : register(t0);//�e�N�X�`���[�O��
//DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);//�e�N�X�`���T���v

static const float division = 768;
static const float blackinterval = 6;
static const int blackheight = 1;
static const float noisewidth = 0.01;

void main(in PS_IN In, out half4 outDiffuse : SV_Target)
{
	//���Y���W�𕪊�
	int divisionindex = In.TexCoord.y * division;

	//���Ԋu�ŉ��ɋ�؂����u���b�N�����
	int noiseindex = divisionindex / blackinterval;

	//�u���b�N���Ƃɉ��ɂ��炷���W�����߂�O����
	//���Ԃɂ�闐���̃V�[�h�l�itime�Ɍ݂��ɑf���ۂ������������̂𕡐��p�ӂ���Ƃ��������ɂȂ�j
	float3 timenoise = float3(0, int(_SinTime.w * 61), int(_SinTime.w * 83));
	//�Ƃ��ǂ��傫�����炷�i���Ԃ̃V�[�h�l���ύX����邽�т�5%�̊m���Ńm�C�Y��10�{�ɂȂ�j
	float noiserate = rand(timenoise) < 0.05 ? 10 : 1;

	//���ɂ��炷�傫���𗐐��Ō��߂�i0~1�j�i���ԓI�ɂ��ʒu�I�ɂ������_���ɂȂ�悤�Ɉʒu�ɂ��V�[�h�l�Ǝ��Ԃɂ��V�[�h�l��ʎ����ŗ^����j
	float xnoise = rand(float3(noiseindex, 0, 0) + timenoise);
	xnoise = xnoise * xnoise - 0.5;             //�����2�悵��0.5�����i2�悵�Ȃ��Ɨ��ꂷ����C�������j
	xnoise = xnoise * noisewidth * noiserate;   //����ɃX�P�[����������
	xnoise = xnoise * (Parameter.x / 2 + 1.1);   //���ԓI�ɂ���ɔg������悤�ɂ���i���������ɂȂ�C������j
	xnoise = xnoise + (abs((int(_SinTime.w * 2000) % int(division / blackinterval)) - noiseindex) < 5 ? 0.005 : 0);    //���X�^�[�X�L�������ۂ��m�C�Y

	float2 uv = In.TexCoord + float2(xnoise, 0);

	//�ڂ₯������
	half4 col1 = g_Texture.Sample(g_SamplerState, uv);;
	half4 col2 = g_Texture.Sample(g_SamplerState, uv + float2(0.005, 0));
	half4 col3 = g_Texture.Sample(g_SamplerState, uv + float2(-0.005, 0));
	half4 col4 = g_Texture.Sample(g_SamplerState, uv + float2(0, 0.005));
	half4 col5 = g_Texture.Sample(g_SamplerState, uv + float2(0, -0.005));
	half4 col = (col1 * 4 + col2 + col3 + col4 + col5) / 8;

	col.rgb = divisionindex % blackinterval < blackheight ? float4(0, 0, 0, 1) : col.rgb;
	outDiffuse = col;
}