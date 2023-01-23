//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"//�}�`�◐���p�C���N���[�h
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//1 �s�N�Z�����̐F�����肵�ďo�͂���
	//���͂��ꂽ�s�N�Z���̐F�����̂܂܏o��
	float2 UV = In.TexCoord;
	outDiffuse = float4(0.f, 0.f, 0.f, 1.f);

	float scan = fan_shape(UV, 50.f) * grad_rot(UV) + grid(UV);
	outDiffuse.g = scan;

	outDiffuse.r *= lerp(1.0f, 0.12f, 1.f - scan);
	outDiffuse.g *= lerp(1.0f, 0.12f, 1.f - scan);
	outDiffuse.b = lerp(0.0f, 0.58f, 1.f - scan);
	outDiffuse.a = lerp(1.f, 0.5f, 1.f - scan);
	//�~�`�̊O���𓧖���
	outDiffuse.a *= lerp(0.0f, 1.f, 1.f - disc(UV, 0.5f));
}