//==============================================================================
// Filename: manager.h
// Description: �}�l�[�W���[�N���X�̒�`
//==============================================================================
#pragma once
#include <unordered_map>
#include <iostream>
#include "Scene.h"
#include "Result_S.h"
#include "Game_S.h"
#include "MainMenu.h"
#include "model.h"
#include "Common.h"

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
	LIGHT_ON,
	LIGHT_OFF,
	LIGHT_LIM,
	NORMAL_TOON,
	NORMAL_FOG,
	SHADER_E
};

//�����
enum class WEPON_TYPE
{
	GATLING,
	LASER,
	BAZOOKA,
	SHOTGUN_1,
};

//�ړ���
enum class MOVE_TYPE
{
	CAR,
	FREE
};

//�Q�[���̓����}�l�[�W���[
class Manager
{
private:
	static class Scene* NowScene;
	static std::list<Scene*> addScene;
	static class Common* common;

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
	static void StartUp()
	{
		//�u�����h�X�e�[�g�ݒ�
		D3D11_BLEND_DESC blend{};
		//SORT_TRUE
		{
			blend.AlphaToCoverageEnable = TRUE;
			blend.IndependentBlendEnable = TRUE;
			blend.RenderTarget[0].BlendEnable = TRUE;
			blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::SORT_TRUE]);
		}

		//SORT_FALSE
		{
			blend.AlphaToCoverageEnable = FALSE;
			blend.IndependentBlendEnable = TRUE;
			blend.RenderTarget[0].BlendEnable = TRUE;
			blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::SORT_FALSE]);
		}

		//OBJ_TRANS
		{
			blend.AlphaToCoverageEnable = FALSE;
			blend.IndependentBlendEnable = TRUE;
			blend.RenderTarget[0].BlendEnable = TRUE;
			blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;	//�������ɂȂ�
			blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::OBJ_TRANS]);
		}

		//OBJ_OPAQUE
		{
			blend.AlphaToCoverageEnable = FALSE;
			blend.IndependentBlendEnable = TRUE;
			blend.RenderTarget[0].BlendEnable = TRUE;
			blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			Renderer::GetDevice()->CreateBlendState(&blend, &B_State[(int)BLEND_S::OBJ_OPAQUE]);
		}

		// ���X�^���C�U�X�e�[�g�ݒ�
		D3D11_RASTERIZER_DESC rasterizerDesc{};
		//WIRE_FRAME
		{
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = FALSE;
			Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::WIRE_FRAME]);
		}

		//CULL_BACK
		{
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = FALSE;
			Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_BACK]);
		}

		//CULL_FLONT
		{
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = FALSE;
			Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_FLONT]);
		}

		//CULL_NONE
		{
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.MultisampleEnable = FALSE;
			Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &rs[(int)FRAME_S::CULL_NONE]);
		}

		//�V�F�[�_�[�ݒ�
			//LIGHT_OFF
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_OFF], &m_VertexLayout[(int)SHADER_S::LIGHT_OFF], "unlitTextureVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_OFF], "unlitTexturePS.cso");
		}

		//LIGHT_ON
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_ON], &m_VertexLayout[(int)SHADER_S::LIGHT_ON], "vertexLightingVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_ON], "vertexLightingPS.cso");
		}

		//LIGHT_LIM
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LIGHT_LIM], &m_VertexLayout[(int)SHADER_S::LIGHT_LIM], "pixelLightingVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LIGHT_LIM], "pixelLightingPS.cso");
		}

		//NORMAL_TOON
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL_TOON], &m_VertexLayout[(int)SHADER_S::NORMAL_TOON], "toonVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL_TOON], "toonPS.cso");
		}

		//NORMAL_FOG
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::NORMAL_FOG], &m_VertexLayout[(int)SHADER_S::NORMAL_FOG], "FogVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::NORMAL_FOG], "FogPS.cso");
		}
	}
	/// ------------------------------------------

	/// �v���C���[�Ǘ�----------------------------
	//�ړ���
	static MOVE_TYPE legtype;

	//�����
	static WEPON_TYPE wepon;
	///-------------------------------------------

public:
	//������
	static void Init();

	//�I������
	static void Uninit();

	//�X�V����
	static void Update();

	//�`�揈��
	static void Draw();

	//���ݎg�p���Ă���V�[���̎擾
	static class Scene* GetScene() { return NowScene; };

	//�V�[���̐ݒ�
	template<typename T>
	static void SetScene()
	{
		if (NowScene)
		{
			NowScene->Uninit();
			delete NowScene;
			DeleteAddScenes();
		}

		NowScene = new T();
		NowScene->Init();
	};

	//�t�F�[�h�V�[���ݒ�
	template<typename T>
	static Common* SetCommon()
	{
		if (common == NULL)
		{
			common = new T();
			common->Init();
		}

		return common;
	}

	//�t�F�[�h�V�[���̎擾
	static Common* GetCommon() { return common; };

	/// �v���C���[�Ǘ�----------------------------
	//�ړ���̎擾
	static MOVE_TYPE GetDSystem() { return legtype; }

	//�����̎擾
	static WEPON_TYPE GetWepontype() { return wepon; }

	//�ړ���̐ݒ�
	static void SetDsystem(MOVE_TYPE in) { legtype = in; }

	//�����̐ݒ�
	static void SetWeponType(WEPON_TYPE in) { wepon = in; }
	///-------------------------------------------

	//�ǉ��V�[���̒ǉ�
	template<typename T>
	static T* AddScene()
	{
		T* scene = new T();
		scene->Init();
		addScene.push_back(scene);
		return scene;
	};

	//�w�肵���ǉ��V�[���̎擾
	template <typename T>
	static T* GetAddScene()
	{
		for (Scene* sce : addScene)
		{
			if (typeid(*sce) == typeid(T))//�^�𒲂ׂ�
			{
				return (T*)sce;
			}
		}
		return NULL;
	}

	//�w�肵���ǉ��V�[���̍폜
	template<typename T>
	static bool DeleteAddScene()
	{
		for (Scene* sce : addScene)
		{
			if (typeid(*sce) == typeid(T))
			{
				sce->Uninit();
				addScene.remove(sce);
				return true;
			}
		}
		return false;
	}

	//�ǉ��V�[���̈ꊇ�폜
	static void DeleteAddScenes()
	{
		std::list<Scene*>::iterator it = addScene.begin();
		for (; it != addScene.end(); ++it) {
			delete* it;
		}
		addScene.clear();
	}

	//���f�����\�[�X�̒ǉ�
	static Model* AddModel(const char* Filename)
	{
		Model* model = NULL;
		if (Modelname.empty())//���g���󂾂����ꍇ
		{
			//���f���̃��[�h�Ɠo�^
			Modelname.insert(nametype::value_type(Filename, models.size()));
			model = new Model();
			model->Load(Filename);
			models.push_back(model);
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//�V�������f���������ꍇ
			{
				//���f���̃��[�h�Ɠo�^
				model = new Model();
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
	static int GetModelKey(const char* Filename)
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
	static int GetTexKey(const char* Filename)
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

	//�Ǘ�����Ă��郂�f���̈ꊇ�폜
	static void DeleteModels()
	{
		for (Model* model : models)
		{
			model->Unload();
			delete(model);
		}
		models.erase(models.begin(), models.end());
	}

	//�X�e�[�g�̈ꊇ�J��
	static void ReleaseState()
	{
		for (int i = 0; i < (int)BLEND_S::BLEND_E; i++)
			B_State[i]->Release();

		for (int i = 0; i < (int)FRAME_S::FRAME_E; i++)
			rs[i];

		for (int i = 0; i < (int)SHADER_S::SHADER_E; i++)
		{
			m_VertexShader[i];
			m_PixelShader[i];
			m_VertexLayout[i];
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
