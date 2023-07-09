//==============================================================================
// Filename: ResourceManager.h
// Description: ���\�[�X�Ǘ��N���X�̒�`
//==============================================================================
#pragma once
#include <unordered_map>
#include <iostream>
#include "main.h"
#include "renderer.h"
#include "model.h"

//���\�[�X�Ǘ��p�̒�`
typedef std::unordered_map<const char*, int> nametype;

//�u�����h�X�e�[�g���
enum class BLEND_S {
	SORT_TRUE, //�\�[�g����
	SORT_FALSE,//�\�[�g�Ȃ�
	OBJ_TRANS, //������
	OBJ_OPAQUE,//�s����
	BLEND_E
};

//���X�^���C�U�X�e�[�g���
enum class FRAME_S {
	WIRE_FRAME, //���C���t���[���\��
	CULL_BACK, //���ʕ\��
	CULL_FLONT,//�\�ʕ\��
	CULL_NONE,//���\�\��
	FRAME_E
};

//�V�F�[�_�[���
enum class SHADER_S
{
	LIGHT_ON, //�����L�� �@���o�͂���
	LIGHT_OFF,//�����Ȃ� �@���o�͂���
	LIGHT_VER_ON,//���_����
	LIGHT_SIMPLE_OFF,//�����Ȃ� �@���o�͂Ȃ�
	LIGHT_LIM,//�������C�g
	NORMAL_TOON,//�v���O�����Ńg�D�[��
	NORMAL_FOG, //�ʏ�t�H�O
	INVISIBLE_FAR,//�����ɂ�铧����
	LOD_SHADOW,//�e�N�X�`���𓧖���
	MOSAIC,//���U�C�N
	GLITCH_NOISE,//�O���b�`�m�C�Y
	JAGGY_NOISE,//�W���M�[�t���O���b�`�m�C�Y
	VIDEO_NOISE,//�r�f�I���m�C�Y
	EDGE,//�G�b�W�V�F�[�_�[
	METAMOL,//�C�m�V�F�[�_�[�i���j
	SCAN,//�~�`���[�_�[�V�F�[�_�[
	DEPTH_SHADOW,//�[�x�o�b�t�@�V���h�E�V�F�[�_�[
	NORMAL_MAPING,//�@���}�b�s���O�V�F�[�_�[
	NORMAL,//�@���o�̓V�F�[�_�[
	SIMPLE_SKY,//�ȈՋ�V�F�[�_�[
	INSTANCE_LIGHT_ON,//�C���X�^���V���O��������V�F�[�_�[
	INSTANCE_Y_BILLBOAD,//�C���X�^���V���OY�Œ�r���{�[�h
	NOISE_CLOUD,//�m�C�Y�_�V�F�[�_�[
	BREND_TEX,//�e�N�X�`���u�����h
	SHADER_E//�V�F�[�_�[�̍ő吔
};

class ResourceManager
{
private:
	/// ���\�[�X�Ǘ�-----------------------------
	//���f���Ǘ�
	static nametype Modelname;
	static std::vector<Model*>models;

	//2D�e�N�X�`���Ǘ�
	static nametype TextureName;
	static std::vector<ID3D11ShaderResourceView*> Texture;
	///------------------------------------------

	///�X�e�[�g�Ǘ�------------------------------
	//�Ăяo���p�u�����h�X�e�[�g
	static ID3D11BlendState* B_State[(int)BLEND_S::BLEND_E];

	//�Ăяo���p���X�^���C�U�X�e�[�g
	static ID3D11RasterizerState* rs[(int)FRAME_S::FRAME_E];

	//�Ăяo���p�V�F�[�_�[
	static ID3D11VertexShader* m_VertexShader[(int)SHADER_S::SHADER_E];
	static ID3D11PixelShader* m_PixelShader[(int)SHADER_S::SHADER_E];
	static ID3D11InputLayout* m_VertexLayout[(int)SHADER_S::SHADER_E];
	///-------------------------------------------

		///�Ăяo���ݒ�̎��O�ǂݍ���-----------------
	static void StartUp();

public:
	static void Init();
	static void Uninit();

	//���f�����\�[�X�̒ǉ�
	static Model* AddModel(const char* Filename)
	{
		Model* model = NULL;
		if (Modelname.empty())//���g���󂾂����ꍇ
		{
			//���f���̃��[�h�Ɠo�^
			Modelname.insert(nametype::value_type(Filename, models.size()));
			model = DBG_NEW Model();
			model->Load(Filename);
			models.push_back(model);
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//�V�������f���������ꍇ
			{
				//���f���̃��[�h�Ɠo�^
				model = DBG_NEW Model();
				model->Load(Filename);
				models.push_back(model);
			}
			else
			{
				//�o�^�ς݂̃��f�����Ăяo��
				int ret = Modelname.at(Filename);
				model = models[ret];
			}
		}

		return model;
	}

	//���f���̎擾
	static Model* GetModel(int key)
	{
		int limit = models.size();
		if (key < limit)
			return models[key];

		return NULL;
	}

	//�w�肵�����f���̌����L�[�̎擾 -1�Ȃ瑶�݂��Ȃ�
	static const int GetModelKey(const char* Filename)
	{
		if (Modelname.empty())//���g���󂾂����ꍇ1
		{
			return -1;
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//�V�������f���������ꍇ
			{
				return -1;
			}
			else
			{
				int ret = Modelname.at(Filename);
				return ret;
			}
		}
	}

	//2D�e�N�X�`���̓o�^
	static ID3D11ShaderResourceView* AddTex(const char* File)
	{
		ID3D11ShaderResourceView* tex = NULL;
		if (TextureName.empty())//���g���󂾂����ꍇ
		{
			//�e�N�X�`���̓ǂݍ��݂Ɠo�^
			TextureName.insert(nametype::value_type(File, Texture.size()));

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				File,
				NULL,
				NULL,
				&tex,
				NULL);

			assert(tex);

			Texture.push_back(tex);
		}
		else
		{
			if (TextureName.insert(nametype::value_type(File, Texture.size())).second)//�V�������f���������ꍇ
			{
				//�e�N�X�`���̓ǂݍ��݂Ɠo�^
				D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
					File,
					NULL,
					NULL,
					&tex,
					NULL);

				assert(tex);

				Texture.push_back(tex);
			}
			else
			{
				//�o�^�ς݂�2D�e�N�X�`�����Ăяo��
				int ret = TextureName.at(File);
				tex = Texture[ret];
			}
		}

		return tex;
	}

	//2D�e�N�X�`���̎擾
	static ID3D11ShaderResourceView* GetTex(int key)
	{
		int limit = Texture.size();
		if (key < limit)
			return Texture[key];

		return NULL;
	}

	//�w�肵��2D�e�N�X�`���̌����L�[�̎擾�@-1�Ȃ瑶�݂��Ȃ�
	static const int GetTexKey(const char* Filename)
	{
		if (TextureName.empty())//���g���󂾂����ꍇ1
		{
			return -1;
		}
		else
		{
			if (TextureName.insert(nametype::value_type(Filename, Texture.size())).second)//�V�������f���������ꍇ
			{
				return -1;
			}
			else
			{
				int ret = TextureName.at(Filename);
				return ret;
			}
		}
	}

	//�Ǘ�����Ă��郂�f���̈ꊇ�J��
	static void DeleteModels()
	{
		for (Model* model : models)
		{
			model->Unload();
			delete(model);
		}
		models.erase(models.begin(), models.end());
		Modelname.clear();
	}

	//�Ǘ�����Ă��郂�f���̈ꊇ�J��
	static void DeleteTextures()
	{
		for (ID3D11ShaderResourceView* texture : Texture)
		{
			texture->Release();
			texture = nullptr;
		}
		Texture.erase(Texture.begin(), Texture.end());
		TextureName.clear();
	}

	//�X�e�[�g�̈ꊇ�J��
	static void ReleaseState()
	{
		for (int i = 0; i < (int)BLEND_S::BLEND_E; i++)
			B_State[i]->Release();

		for (int i = 0; i < (int)FRAME_S::FRAME_E; i++)
			rs[i]->Release();

		for (int i = 0; i < (int)SHADER_S::SHADER_E; i++)
		{
			m_VertexShader[i]->Release();
			m_PixelShader[i]->Release();
			m_VertexLayout[i]->Release();
		}
	}

	//�u�����h�X�e�[�g�̎擾
	static ID3D11BlendState* GetBlend(BLEND_S state)
	{
		if (state < BLEND_S::BLEND_E && state >= (BLEND_S)0)
		{
			return B_State[(int)state];
		}
		return NULL;
	}

	//���X�^���C�U�X�e�[�g�̎擾
	static ID3D11RasterizerState* GetFrame(FRAME_S state)
	{
		if (state < FRAME_S::FRAME_E && state >= (FRAME_S)0)
		{
			return rs[(int)state];
		}
		return NULL;
	}

	//�V�F�[�_�[�̎擾
	static void GetShaderState(ID3D11VertexShader** inver, ID3D11PixelShader** inpix, ID3D11InputLayout** inlay, SHADER_S type)
	{
		SHADER_S in = SHADER_S::LIGHT_OFF;

		if (type < SHADER_S::SHADER_E && type >= (SHADER_S)0)
			in = type;

		*inver = m_VertexShader[(int)in];
		*inpix = m_PixelShader[(int)in];
		*inlay = m_VertexLayout[(int)in];
	}
};
