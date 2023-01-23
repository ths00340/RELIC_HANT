//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//

Texture2D g_Texture : register(t0);//�ʏ�e�N�X�`��
Texture2D g_TextureShadowDepth : register(t1);//�V���h�E�}�b�v
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�@���}�b�s���O
	{
		float4 normalmap = In.Normal * 2.f - 1.f;
		normal.x = -normalmap.r;
		normal.y = normalmap.b;
		normal.z = normalmap.g;
		normal.w = 0.f;
	}

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//�����v�Z������
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;
	light = clamp(light, 0.5f, 1.f);

	//�C�ʂ̍���
	float sea_height = In.WorldPosition.y - 1.f;

	float r = 0.02;
	float facing = saturate(1.f - dot(normal, eyev));
	float fresnel = r + (1.0 - r) * pow(facing, 5.0);

	float3 sea_base_color = float3(0.105f, 0.223f, 0.301f) * light * 1.5f;
	float3 sea_color = sea_base_color + (float3(0.294f, 0.349f, 0.137f) * (sea_height * 0.5 + 0.2) * 0.75f);

	//�x�[�X�̃s�N�Z���F���擾
	outDiffuse = float4(sea_color, 1.f);

	//�V���h�E�}�b�v
	{
		In.ShadowPosition.xyz /= In.ShadowPosition.w; //���e�ʂł̃s�N�Z���̍��W����
		In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //�e�N�X�`�����W�ɕϊ�
		In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //

			//�V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
		float depth = g_TextureShadowDepth.Sample(g_SamplerState,In.ShadowPosition.xy);
		outDiffuse.rgb = lerp(outDiffuse.rgb, outDiffuse.rgb * 0.5f,step(depth, In.ShadowPosition.z - 0.001));
	}

	//�����v�Z
	{
		//���̔��˃x�N�g�����v�Z
		float3 refv = reflect(Light.Direction.xyz, normal.xyz);
		refv = normalize(refv); //���K������

		//���ʔ��˂̌v�Z
		float specular = -dot(eyev, refv);

		//�l���T�`�����[�g
		specular = saturate(specular);

		//�����ł͂R�O�悵�Ă݂�
		specular = pow(specular, 100);

		//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������
		outDiffuse.rgb += specular;
	}
}