//�s�N�Z���V�F�[�_�[
#include "common.hlsl" //�K���C���N���[�h
#include "PainterTool.hlsl"
//
//in �͓��͂���Ă���f�[�^�[
//out �͏o�͂���f�[�^
//
void main(in PS_IN In, out PS_OUT Out)
{
	int Grid = 40;
	float StarSize = 0.05f;
	float Luminance = 1000.f;//���̋���
	float3 SatellitePos = Light.Position;
	float4 TopColor, UnderColor, MoonColor;
	TopColor = float4(0.058f, 0.07f, 0.149f, 1.f);//��float4(0.686f, 0.933f, 0.933f, 1.f);//
	UnderColor = float4(0.203f, 0.239f, 0.45f, 1.f);//��float4(0.878f, 1.f, 1.f, 1.f);
	MoonColor = float4(0.968f, 0.972f, 0.941f, 1.f);
	Out.Normal = float4(1.f, 0.f, 0.f, 1.f);
	//float4(0.686f,0.933f,0.933f,1.f);  //��
	//float4(0.878f,1.f,1.f,1.f);        //��

	//�`�悵�����s�N�Z���̃��[�J�����W�𐳋K��
	float3 dir = normalize(In.WorldPosition);
	//���W�A�����Z�o����
	float2 rad = float2(atan2(dir.x, dir.z), asin(dir.y));
	float2 uv = rad / float2(3.141592f / 2.f, 3.141592f / 2.f);

	uv *= Grid; //�i�q��̃}�X�ڍ쐬 UV�ɂ�����������������UV���J��Ԃ��W�J�����

	float2 ist = floor(uv); //�e�}�X�ڂ̋N�_
	float2 fst = frac(uv); //�e�}�X�ڂ̋N�_����̕`�悵�����ʒu

	float4 color = 0;

	//���g�܂ގ��͂̃}�X��T��
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			//����1�~1�̃G���A
			float2 neighbor = float2(x, y);

			//�_��xy���W
			float2 p = rand2(ist);

			//�_�Ə����Ώۂ̃s�N�Z���Ƃ̋����x�N�g��
			float2 diff = neighbor + p - fst;

			//�F�𐯂��ƂɃ����_���ɓ��Ă͂߂�
			float r = rand(p + 1);
			float g = rand(p + 2);
			float b = rand(p + 3);
			float4 randColor = float4(r, g, b, 1);

			//"�_"��"���ݕ`�悵�悤�Ƃ��Ă���s�N�Z���Ƃ̋���"�𗘗p���Đ���`�悷�邩�ǂ������v�Z
			//step(t,x) ��x��t���傫���ꍇ1��Ԃ�
			float interpolation = 1 - step(rand(p) * StarSize, length(diff));
			Out.Diffuse = lerp(color, randColor, interpolation);
		}
	}

	//������UV��Y�������̍��W�𗘗p���ĐF���O���f�[�V����������
	Out.Diffuse += lerp(UnderColor, TopColor, dir.y + 0.75f);
	//��
	{
		float3 dir2 = normalize(SatellitePos); //���z�̈ʒu�x�N�g�����K��
		float angle = dot(dir2, dir); //���z�̈ʒu�x�N�g���@�Ɓ@�`�悳���s�N�Z���̈ʒu�x�N�g���@�̓���
		//_SunStrength��傫������قǌv�Z���ʂ�0�ɋ߂Â�
		float3 c = Out.Diffuse + (MoonColor * pow(max(0, angle), Luminance) * 1.f);
		Out.Diffuse.rgb = c;
	}
}