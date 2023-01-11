//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//
//DirectX�̃e�N�X�`���̐ݒ���󂯌p��
Texture2D g_Texture : register(t0);//�e�N�X�`���[�O��
//DirectX�̃T���v���[�X�e�[�g�ݒ���󂯌p��
SamplerState g_SamplerState : register(s0);//�e�N�X�`���T���v

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord;
    //�|�X�^���C�Y 
    float posterize1 = floor(frac(perlinNoise(_SinTime.xy) * 10.f) / (1.f / Parameter.y)) * (1.f / Parameter.y);
    float posterize2 = floor(frac(perlinNoise(_SinTime.xy) * 5.f) /  (1.f / Parameter.y)) * (1.f / Parameter.y);
    //uv.x�����̃m�C�Y�v�Z -0.1 < noiseX < 0.1
    float noiseX = (2.f * rand(posterize1) - 0.5f) * 0.1f;
    //step(t,x) ��x��t���傫���ꍇ1��Ԃ�
    float frequency = step(rand(posterize2), Parameter.x);
    noiseX *= frequency;
    //uv.y�����̃m�C�Y�v�Z -1 < noiseY < 1
    float noiseY = 2.f * rand(posterize1) - 0.5f;
    //�O���b�`�̍����̕�Ԓl�v�Z �ǂ̍����ɏo�����邩�͎��ԕω��Ń����_��
    float glitchLine1 = step(uv.y - noiseY, rand(uv));
    float glitchLine2 = step(uv.y + noiseY, noiseY);
    float glitch = saturate(glitchLine1 - glitchLine2);
    //���x����
    uv.x = lerp(uv.x, uv.x + noiseX, glitch);
    //�e�N�X�`���T���v�����O
    outDiffuse = g_Texture.Sample(g_SamplerState,uv);
}