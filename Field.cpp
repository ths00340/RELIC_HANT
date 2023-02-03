#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Field.h"

void Field::Init()
{
	name = "Field";
	m_pos = Float3(0.0f, 0.0f, 0.0f);
	m_scl = Float3(1.0f, 1.0f, 1.0f);

	numVertex = (CHIP_Y + 1) * (CHIP_X + 1);
	numIndex = ((4 + (2 * (CHIP_X - 1))) * CHIP_Y) + (2 * (CHIP_Y - 1));

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * numVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * numIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_IndexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	unsigned short* pIdx = (unsigned short*)msr.pData;

	//n���ɑΉ��ł���C���f�b�N�X�̐ݒ���l���Ă݂܂��傤�I
	int I = 0;
	int J = 0;
	int count = 0;
	for (int i = 0; i < CHIP_Y; i++)
	{
		for (int j = 0; j < CHIP_X + 1; j++)
		{
			pIdx[I * 2 + 0] = (CHIP_X + 1) + J;
			pIdx[(I * 2) + 1] = J;

			if ((I % 2) == 0)
			{
				Mai[count].a = pIdx[I * 2 + 0];
				Mai[count].b = pIdx[(I * 2) + 1];
			}
			if ((I % 2) == 1)
			{
				Mai[count].c = pIdx[I * 2 + 0];
				Mai[count].d = pIdx[(I * 2) + 1];
				count++;
			}
			I++;
			J++;
		}
		pIdx[(I * 2) + 0] = J - 1;
		pIdx[(I * 2) + 1] = (CHIP_X + 1) + J;
		I++;
	}

	Renderer::GetDeviceContext()->Unmap(m_IndexBuffer, 0);

	{//���_�o�b�t�@�̒��g�𖄂߂�
// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		pVtx = (VERTEX_3D*)msr.pData;

		//���ɂ�2�񃋁[�v����
		for (int y = 0; y < (CHIP_Y + 1); y++)
		{
			//y_t = rand() % 50;
			//�c�ɂ͕K�v����+1�񃋁[�v����
			for (int x = 0; x < (CHIP_X + 1); x++)
			{
				//�z��̃C���f�b�N�X���v�Z����
				int i = y * (CHIP_X + 1) + x;

				pVtx[i].Position = D3DXVECTOR3(0.0f + (x * CHIP_SIZE_X) - ((CHIP_X / 2) * CHIP_SIZE_X), 0.0f, 0.0f - (y * CHIP_SIZE_Y) + ((CHIP_Y / 2) * CHIP_SIZE_X));
				// UV�l�̐ݒ�
				pVtx[i].TexCoord = D3DXVECTOR2(0.0f + x * 1.0f, 0.0f + y * 1.0f);

				// �@���̐ݒ�
				pVtx[i].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// ���_�J���[�̐ݒ�
				pVtx[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		for (int y = 1; y < (CHIP_Y); y++)
			for (int x = 1; x < (CHIP_X + 1); x++)
			{
				int i = y * (CHIP_X + 1) + x;
				Float3 vx, vz, vn;
				vz = pVtx[i + 1].Position - pVtx[i - 1].Position;
				vx = pVtx[i + CHIP_X].Position - pVtx[i - CHIP_X].Position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				pVtx[i].Normal = vn;

				vz = (pVtx[i + 1].Position + pVtx[i - 1].Position);
				vx = (pVtx[i + CHIP_X].Position + pVtx[i - CHIP_X].Position);

				float h = (vz.y + vx.y) * 0.25f;
				pVtx[i].Position.y = h + ((TOOL::RandF() * 0.5f));
			}

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	//�e�N�X�`���ǂݍ���

	m_Texture = ResourceManager::AddTex("asset/texture/sand.jpg");

	//�V�F�[�_�[�֌W
	ResourceManager::GetShaderState(&m_VertexShader, &m_PixelShader, &m_VertexLayout, SHADER_S::LIGHT_ON);

	m_pos = Float3(0.f, 0.f, 0.f);
	m_scl = Float3(1.f, 1.f, 1.f);
	m_rot = Float3(0.f, 0.f, 0.f);

	blendState = ResourceManager::GetBlend(BLEND_S::OBJ_OPAQUE);
}

void Field::Uninit()
{}

void Field::Update()
{}

void Field::Draw()
{
	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[����
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scl, rot, trans;
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	world = scl * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	ID3D11ShaderResourceView* DepthTexture = Renderer::GetShadowDepthTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1,&DepthTexture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed(numIndex, 0, 0);
}

float Field::GetHeight(Float3 inPos)
{
	int x, z;

	x = (inPos.x / CHIP_SIZE_X) + (CHIP_X * 0.5f);
	z = -(inPos.z / CHIP_SIZE_Y) - (CHIP_Y * 0.5f);
	z += CHIP_Y - 1;

	int i = z * (CHIP_X + 1) + x;
	int j = ((z + 1) * (CHIP_X + 1)) + x;

	Float3 pos0, pos1, pos2, pos3;
	if (x >= CHIP_X || z >= CHIP_Y || x <= 0 || z <= 0)
		return 0.f;

	pos0 = pVtx[i].Position;
	pos1 = pVtx[i + 1].Position;
	pos2 = pVtx[j].Position;
	pos3 = pVtx[j + 1].Position;

	Float3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = inPos - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);//���v���Ōv�Z����ƕ\����

	float py;
	Float3 n;

	if (c.y > 0.f)
	{
		Float3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
		TOOL::Display((char*)"���㪁I\n");
	}
	else
	{
		Float3 v13;
		v13 = pos1 - pos3;
		D3DXVec3Cross(&n, &v12, &v13);
		TOOL::Display((char*)"�E����I\n");
	}

	py = -((inPos.x - pos1.x) * n.x
		+ (inPos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}