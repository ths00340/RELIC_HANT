//==============================================================================
// Filename: ResourceManager.h
// Description: リソース管理クラスの定義
//==============================================================================
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
	LIGHT_ON, //光源有り 法線出力あり
	LIGHT_OFF,//光源なし 法線出力あり
	LIGHT_VER_ON,//頂点光源
	LIGHT_SIMPLE_OFF,//光源なし 法線出力なし
	LIGHT_LIM,//リムライト
	NORMAL_TOON,//プログラムでトゥーン
	NORMAL_FOG, //通常フォグ
	INVISIBLE_FAR,//距離による透明化
	LOD_SHADOW,//テクスチャを透明に
	MOSAIC,//モザイク
	GLITCH_NOISE,//グリッチノイズ
	JAGGY_NOISE,//ジャギー付きグリッチノイズ
	VIDEO_NOISE,//ビデオ風ノイズ
	EDGE,//エッジシェーダー
	METAMOL,//海洋シェーダー（仮）
	SCAN,//円形レーダーシェーダー
	DEPTH_SHADOW,//深度バッファシャドウシェーダー
	NORMAL_MAPING,//法線マッピングシェーダー
	NORMAL,//法線出力シェーダー
	SIMPLE_SKY,//簡易空シェーダー
	INSTANCE_LIGHT_ON,//インスタンシング光源ありシェーダー
	INSTANCE_Y_BILLBOAD,//インスタンシングY固定ビルボード
	NOISE_CLOUD,//ノイズ雲シェーダー
	BREND_TEX,//テクスチャブレンド
	SHADER_E//シェーダーの最大数
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
	static void StartUp();

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
			model = DBG_NEW Model();
			model->Load(Filename);
			models.push_back(model);
		}
		else
		{
			if (Modelname.insert(nametype::value_type(Filename, models.size())).second)//新しいモデルだった場合
			{
				//モデルのロードと登録
				model = DBG_NEW Model();
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
		Modelname.clear();
	}

	//管理されているモデルの一括開放
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

	//ステートの一括開放
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
