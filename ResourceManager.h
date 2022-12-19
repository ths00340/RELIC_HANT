#pragma once
#include <unordered_map>
#include <iostream>
#include "main.h"
#include "renderer.h"
#include "model.h"

//リソース管理用の定義
typedef std::unordered_map<const char*, int> nametype;

//ブレンドステート種類
enum class BLEND_S {
	SORT_TRUE, //ソートあり
	SORT_FALSE,//ソートなし
	OBJ_TRANS, //半透明
	OBJ_OPAQUE,//不透明
	BLEND_E
};

//ラスタライザステート種類
enum class FRAME_S {
	WIRE_FRAME, //ワイヤフレーム表示
	CULL_BACK, //裏面表示
	CULL_FLONT,//表面表示
	CULL_NONE,//裏表表示
	FRAME_E
};

//シェーダー種類
enum class SHADER_S
{
	LIGHT_ON,
	LIGHT_OFF,
	LIGHT_LIM,
	NORMAL_TOON,
	NORMAL_FOG,
	INVISIBLE_FAR,
	LOD_SHADOW,
	SHADER_E
};

class ResourceManager
{
private:
	/// リソース管理-----------------------------
	//モデル管理
	static nametype Modelname;
	static std::vector<Model*>models;

	//2Dテクスチャ管理
	static nametype TextureName;
	static std::vector<ID3D11ShaderResourceView*> Texture;
	///------------------------------------------

	///ステート管理------------------------------
	//呼び出し用ブレンドステート
	static ID3D11BlendState* B_State[(int)BLEND_S::BLEND_E];

	//呼び出し用ラスタライザステート
	static ID3D11RasterizerState* rs[(int)FRAME_S::FRAME_E];

	//呼び出し用シェーダー
	static ID3D11VertexShader* m_VertexShader[(int)SHADER_S::SHADER_E];
	static ID3D11PixelShader* m_PixelShader[(int)SHADER_S::SHADER_E];
	static ID3D11InputLayout* m_VertexLayout[(int)SHADER_S::SHADER_E];
	///-------------------------------------------

		///呼び出し設定の事前読み込み-----------------
	static void StartUp()
	{
		//ブレンドステート設定
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
			blend.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;	//半透明になる
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

		// ラスタライザステート設定
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

		//シェーダー設定
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

		//INVISIBLE_FAR
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::INVISIBLE_FAR], &m_VertexLayout[(int)SHADER_S::INVISIBLE_FAR], "FarInvisibleVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::INVISIBLE_FAR], "FarInvisiblePS.cso");
		}

		//LOD_SHADOW
		{
			Renderer::CreateVertexShader(&m_VertexShader[(int)SHADER_S::LOD_SHADOW], &m_VertexLayout[(int)SHADER_S::LOD_SHADOW], "LODShadowVS.cso");
			Renderer::CreatePixelShader(&m_PixelShader[(int)SHADER_S::LOD_SHADOW], "LODShadowPS.cso");
		}
	}

public:
	static void Init();
	static void Uninit();

	//モデルリソースの追加
	static Model* AddModel(const char* Filename)
	{
		Model* model = NULL;
		if (Modelname.empty())//中身が空だった場合
		{
			//モデルのロードと登録
			Modelname.insert(nametype::value_type(Filename, models.size()));
			model = new Model();
			model->Load(Filename);
			models.push_back(model);
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//新しいモデルだった場合
			{
				//モデルのロードと登録
				model = new Model();
				model->Load(Filename);
				models.push_back(model);
			}
			else
			{
				//登録済みのモデルを呼び出し
				int ret = Modelname.at(Filename);
				model = models[ret];
			}
		}

		return model;
	}

	//モデルの取得
	static Model* GetModel(int key)
	{
		int limit = models.size();
		if (key < limit)
			return models[key];

		return NULL;
	}

	//指定したモデルの検索キーの取得 -1なら存在しない
	static const int GetModelKey(const char* Filename)
	{
		if (Modelname.empty())//中身が空だった場合1
		{
			return -1;
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//新しいモデルだった場合
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

	//2Dテクスチャの登録
	static ID3D11ShaderResourceView* AddTex(const char* File)
	{
		ID3D11ShaderResourceView* tex = NULL;
		if (TextureName.empty())//中身が空だった場合
		{
			//テクスチャの読み込みと登録
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
			if (TextureName.insert(nametype::value_type(File, Texture.size())).second)//新しいモデルだった場合
			{
				//テクスチャの読み込みと登録
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
				//登録済みの2Dテクスチャを呼び出し
				int ret = TextureName.at(File);
				tex = Texture[ret];
			}
		}

		return tex;
	}

	//2Dテクスチャの取得
	static ID3D11ShaderResourceView* GetTex(int key)
	{
		int limit = Texture.size();
		if (key < limit)
			return Texture[key];

		return NULL;
	}

	//指定した2Dテクスチャの検索キーの取得　-1なら存在しない
	static const int GetTexKey(const char* Filename)
	{
		if (TextureName.empty())//中身が空だった場合1
		{
			return -1;
		}
		else
		{
			if (TextureName.insert(nametype::value_type(Filename, Texture.size())).second)//新しいモデルだった場合
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

	//管理されているモデルの一括開放
	static void DeleteModels()
	{
		for (Model* model : models)
		{
			model->Unload();
			delete(model);
		}
		models.erase(models.begin(), models.end());
	}

	//ステートの一括開放
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

	//ブレンドステートの取得
	static ID3D11BlendState* GetBlend(BLEND_S state)
	{
		if (state < BLEND_S::BLEND_E && state >= (BLEND_S)0)
		{
			return B_State[(int)state];
		}
		return NULL;
	}

	//ラスタライザステートの取得
	static ID3D11RasterizerState* GetFrame(FRAME_S state)
	{
		if (state < FRAME_S::FRAME_E && state >= (FRAME_S)0)
		{
			return rs[(int)state];
		}
		return NULL;
	}

	//シェーダーの取得
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
