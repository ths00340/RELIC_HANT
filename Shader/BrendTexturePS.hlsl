#include "common.hlsl"
Texture2D g_Texture				: register(t0);//�ʏ�e�N�X�`��
Texture2D g_TextureShadowDepth	: register(t1);//�V���h�E�}�b�v
Texture2D g_TextureCross		: register(t2);//���}�b�s���O�p�e�N�X�`��
Texture2D g_TextureNormal		: register(t3);//�@���}�b�s���O�p�e�N�X�`��
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�s�N�Z���̖@���𐳋K��
	float4 normal = normalize(In.Normal);
	//�@���}�b�s���O
	{
		float4 normalmap = g_TextureNormal.Sample(g_SamplerState, In.TexCoord+ _SinTime.xy);
		normalmap = normalmap * 2.f - 1.f;
		normal.x = -normalmap.r;
		normal.y = normalmap.b;
		normal.z = normalmap.g;
		normal.w = 0.f;
	}

	//�����v�Z������
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5;;

	//�X�y�L�����̌v�Z
	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//�����ł̐F�����ʂ̃e�N�X�`�������_�F���쐬���Ă���
	outDiffuse = float4(0.f,0.5f,1.f,1.f);

	//���}�b�s���O
	//{
	//	float4 envTex = g_TextureCross.SampleBias(g_SamplerState, In.TexCoord, Parameter.z);
	//	outDiffuse.rgb += envTex;
	//}

	outDiffuse *= In.Diffuse*light; //���邳�ƐF����Z


	//�V���h�E�}�b�s���O
	{
		In.ShadowPosition.xyz /= In.ShadowPosition.w; //���e�ʂł̃s�N�Z���̍��W����
		In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; //�e�N�X�`�����W�ɕϊ�
		In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5; //
	}



	//�V���h�E�}�b�s���O
	{
		//�V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
		float depth = g_TextureShadowDepth.Sample(g_SamplerState,
			In.ShadowPosition.xy);

		//�擾�l���ʏ�J��������s�N�Z���ւ̋�����菬����
		if (depth < In.ShadowPosition.z - 0.0001) //0.01��Z�t�@�C�e�B���O�␳�l�i��q�j
		{
			outDiffuse.rgb *= 0.75f; //�F���Â�����
		}
	}

	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
	specular = saturate(specular); //�l���T�`�����[�g

	specular = pow(specular, 30); //�����ł͂R�O�悵�Ă݂�

	outDiffuse.rgb += specular;//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������

	//float rim = 1.0 + dot(eyev, normal.xyz); //�����Ɩ@���̓��ς𖾂邳�ɕϊ�����
	//rim = pow(rim, 10) * 2.0f; //�X�y�L�����Ɠ����悤�ȏ�����K���ɍs���B
	//rim = saturate(rim); //rim���T�`�����[�g����
	//outDiffuse.rgb += rim;

	outDiffuse.a *= In.Diffuse.a;
}