#include "common.hlsl"
#include "PainterTool.hlsl"
Texture2D g_Texture				: register(t0);//�ʏ�e�N�X�`��
Texture2D g_TextureShadowDepth	: register(t1);//�V���h�E�}�b�v
Texture2D g_Texture2		: register(t2);//�e�N�X�`�����̂Q
Texture2D g_Texture3		: register(t3);//�e�N�X�`�����̂R

SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out PS_OUT Out : SV_Target)
{
	float4 tex1, tex2,tex3;
	tex1 = g_Texture.Sample(g_SamplerState, In.TexCoord);
	tex2 = g_Texture2.Sample(g_SamplerState, In.TexCoord);
	tex3 = g_Texture3.Sample(g_SamplerState, In.TexCoord);

	{//�����t�H�O
		float height = In.WorldPosition.y/15.f;
		float heightfog = saturate(height);
		float r = 0.3f*fBm(In.WorldPosition.xz, 0.f, 3);
		Out.Diffuse = Transition(heightfog-r,0.05f,0.3f,tex1,tex2,tex3);
	}

	Out.Normal = In.Normal;
}