#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "renderer.h"

#include "model.h"

void Model::Draw()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_SubsetArray[i].Material.Texture);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}
}

void Model::InstanceDraw(int num, ID3D11ShaderResourceView* inWpmRSV)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_SubsetArray[i].Material.Texture);
		Renderer::GetDeviceContext()->VSSetShaderResources(3, 1, &inWpmRSV);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexedInstanced(m_SubsetArray[i].IndexNum, num, m_SubsetArray[i].StartIndex, 0, 0);
	}
}

void Model::Load(const char* FileName)
{
	MODEL model;
	LoadObj(FileName, &model);

	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = DBG_NEW SUBSET[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			//�f�o�b�O�p�I���R�[�h
			assert(m_SubsetArray[i].Material.Texture);
		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;
}

void Model::Unload()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		m_SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_SubsetArray;
}

const Float3 Model::Get_max()
{
	Float3 ret = Float3(
		Size_max.x,
		Size_max.y,
		Size_max.z
	);
	return ret;
}

const Float3 Model::Get_min()
{
	Float3 ret = Float3(
		Size_min.x,
		Size_min.y,
		Size_min.z
	);
	return ret;
}

const Float3 Model::Get_total()
{
	Float3 ret = Float3(
		Size_max.x - Size_min.x,
		Size_max.y - Size_min.y,
		Size_max.z - Size_min.z
	);
	return ret;
}

//���f���Ǎ�////////////////////////////////////////////
void Model::LoadObj(const char* FileName, MODEL* Model)
{
	int t = 0;//�x���΍�
	Size_max = Float3(0.0f, 0.0f, 0.0f);
	Size_min = Float3(1000.0f, 1000.0f, 1000.0f);
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);

	D3DXVECTOR3* positionArray;
	D3DXVECTOR3* normalArray;
	D3DXVECTOR2* texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL* materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char* s;
	char c;

	FILE* file;
	file = fopen(FileName, "rt");
	//if (file != NULL)
	//{
	//	int i = 0;
	//}
	assert(file);

	//�v�f���J�E���g
	while (true)
	{
		t = fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				t = fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}

	//�������m��
	positionArray = DBG_NEW D3DXVECTOR3[positionNum];
	normalArray = DBG_NEW D3DXVECTOR3[normalNum];
	texcoordArray = DBG_NEW D3DXVECTOR2[texcoordNum];

	Model->VertexArray = DBG_NEW VERTEX_3D[vertexNum];
	Model->VertexNum = vertexNum;

	Model->IndexArray = DBG_NEW unsigned int[indexNum];
	Model->IndexNum = indexNum;

	Model->SubsetArray = DBG_NEW SUBSET[subsetNum];
	Model->SubsetNum = subsetNum;

	//�v�f�Ǎ�
	D3DXVECTOR3* position = positionArray;
	D3DXVECTOR3* normal = normalArray;
	D3DXVECTOR2* texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		t = fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			//�}�e���A���t�@�C��
			t = fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//�I�u�W�F�N�g��
			t = fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//���_���W
			t = fscanf(file, "%f", &position->x);
			t = fscanf(file, "%f", &position->y);
			t = fscanf(file, "%f", &position->z);

			{
				if (position->x > Size_max.x)
					Size_max.x = position->x;

				if (position->y > Size_max.y)
					Size_max.y = position->y;

				if (position->z > Size_max.z)
					Size_max.z = position->z;
			}

			{
				if (position->x < Size_min.x)
					Size_min.x = position->x;

				if (position->y < Size_min.y)
					Size_min.y = position->y;

				if (position->z < Size_min.z)
					Size_min.z = position->z;
			}

			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//�@��
			t = fscanf(file, "%f", &normal->x);
			t = fscanf(file, "%f", &normal->y);
			t = fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//�e�N�X�`�����W
			t = fscanf(file, "%f", &texcoord->x);
			t = fscanf(file, "%f", &texcoord->y);
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//�}�e���A��
			t = fscanf(file, "%s", str);

			if (sc != 0)
				Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

			Model->SubsetArray[sc].StartIndex = ic;

			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					Model->SubsetArray[sc].Material.Material = materialArray[i].Material;
					strcpy(Model->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(Model->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;
		}
		else if (strcmp(str, "f") == 0)
		{
			//��
			in = 0;

			do
			{
				t = fscanf(file, "%s", str);

				s = strtok(str, "/");
				Model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//�e�N�X�`�����W�����݂��Ȃ��ꍇ������
					s = strtok(NULL, "/");
					Model->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				Model->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				Model->VertexArray[vc].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//�l�p�͎O�p�ɕ���
			if (in == 4)
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}

	if (sc != 0)
		Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

	fclose(file);

	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}

//�}�e���A���ǂݍ���///////////////////////////////////////////////////////////////////
void Model::LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum)
{
	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);

	char str[256];

	FILE* file;
	file = fopen(FileName, "rt");
	if (file == NULL)
	{
		int i = 0;
	}

	assert(file);

	MODEL_MATERIAL* materialArray;
	unsigned int materialNum = 0;

	//�v�f���J�E���g
	while (true)
	{
		int t = fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}

	//�������m��
	materialArray = DBG_NEW MODEL_MATERIAL[materialNum];

	//�v�f�Ǎ�
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		int t = fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "newmtl") == 0)
		{
			//�}�e���A����
			mc++;
			t = fscanf(file, "%s", materialArray[mc].Name);
			strcpy(materialArray[mc].TextureName, "");
		}
		else if (strcmp(str, "Ka") == 0)
		{
			//�A���r�G���g
			t = fscanf(file, "%f", &materialArray[mc].Material.Ambient.r);
			t = fscanf(file, "%f", &materialArray[mc].Material.Ambient.g);
			t = fscanf(file, "%f", &materialArray[mc].Material.Ambient.b);
			materialArray[mc].Material.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//�f�B�t���[�Y
			t = fscanf(file, "%f", &materialArray[mc].Material.Diffuse.r);
			t = fscanf(file, "%f", &materialArray[mc].Material.Diffuse.g);
			t = fscanf(file, "%f", &materialArray[mc].Material.Diffuse.b);
			materialArray[mc].Material.Diffuse.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			//�X�y�L����
			t = fscanf(file, "%f", &materialArray[mc].Material.Specular.r);
			t = fscanf(file, "%f", &materialArray[mc].Material.Specular.g);
			t = fscanf(file, "%f", &materialArray[mc].Material.Specular.b);
			materialArray[mc].Material.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//�X�y�L�������x
			t = fscanf(file, "%f", &materialArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "d") == 0)
		{
			//�A���t�@
			t = fscanf(file, "%f", &materialArray[mc].Material.Diffuse.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//�e�N�X�`��
			t = fscanf(file, "%s", str);

			char path[256];
			strcpy(path, dir);
			strcat(path, "\\");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);
		}
		else if (strcmp(str, "Ni") == 0)
		{
			//�A���t�@
			t = fscanf(file, "%f", &materialArray[mc].Material.Shininess);
		}
		else if (strcmp(str, "Ke") == 0)
		{
			//�A���t�@
			t = fscanf(file, "%f", &materialArray[mc].Material.Emission.r);
			t = fscanf(file, "%f", &materialArray[mc].Material.Emission.g);
			t = fscanf(file, "%f", &materialArray[mc].Material.Emission.b);
			materialArray[mc].Material.Emission.a = 1.0f;
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}