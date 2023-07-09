//==============================================================================
// Filename: Player.h
// Description :GameObjectクラスを継承したプレイヤー用クラスの定義
//==============================================================================
#pragma once
#include "GameObject.h"
#include "Wepon.h"
#include "DriveSystem.h"
#include "Drive.h"
#define SE_MAX_NUM (10)

class NumberManager;
class Model;

class Player :public GameObject
{
private:
	Model* m_model;//モデルポインタ

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_VertexLayout;
	ID3D11BlendState  * blendState;

	//発射音用オーディオポインタ※コンポーネントに移動させるため削除予定
	class Audio* m_pShotSE = nullptr;

	//ステータスコンポーネントポインタ
	Status* m_pSta = nullptr;

	//武器コンポーネントポインタ
	Wepon* m_pWepon = nullptr;

	//移動用(動作)コンポーネントポインタ
	DriveSystem* m_pDriveS = nullptr;

	//移動用(見た目)コンポーネントポインタ
	Drive* m_pDrive = nullptr;

	//HP表示用UIオブジェクトポインタ
	NumberManager* m_pHp = nullptr;

	//MP表示用UIオブジェクトポインタ
	NumberManager* m_pMp = nullptr;

	class Camera* m_pCam = nullptr;

	class Scene* m_pScene = nullptr;

	class cInputOperation* m_pInput;

	//車体の細かな振動用変数
	float m_Idol = 0.0f;
public:
	//コンストラクター
	Player()
	{
		m_pShotSE = nullptr;
		m_model = nullptr;
		m_pSta = nullptr;
		m_pWepon = nullptr;
		m_pDriveS = nullptr;
		m_pDrive = nullptr;
		m_pHp = nullptr;
		m_pMp = nullptr;
		m_Idol = 0.0f;

		m_VertexShader	= nullptr;
		m_PixelShader	= nullptr;
		m_VertexLayout	= nullptr;
		blendState		= nullptr;
		m_pInput		= nullptr;
	}

	void Init();//初期化
	void Uninit();//後処理用関数
	void Update();//更新処理
	void FixedUpdate();//全体の更新後の更新処理
	void Draw();//描画

	//ブレンドステートの読み取り
	ID3D11BlendState* GetBlendState()override { return blendState; }

	//武器コンポーネントの追加
	template <typename T>
	void SetWepon()
	{
		DeleteWepon();
		m_pWepon = AddComponent<T>();
	}

	//移動用コンポーネント(動作)の設定
	template <typename T>
	void SetDriveS()
	{
		DeleteDriveS();
		m_pDriveS = AddComponent<T>();
	}

	//移動用コンポーネント(見た目)の設定
	template <typename T>
	void SetDrive()
	{
		DeleteDrive();
		m_pDrive = AddComponent<T>();
	}

	//武器コンポーネントの削除
	void DeleteWepon()
	{
		if (m_pWepon != nullptr)
		{
			Component.remove(m_pWepon);
			delete m_pWepon;
			m_pWepon = nullptr;
		}
	}

	//移動用コンポーネント(動作)の削除
	void DeleteDriveS()
	{
		if (m_pDriveS != nullptr)
		{
			Component.remove(m_pDriveS);
			delete m_pDriveS;
			m_pDriveS = nullptr;
		}
	}

	//移動用コンポーネント(見た目)の削除
	void DeleteDrive()
	{
		if (m_pDrive != nullptr)
		{
			Component.remove(m_pDrive);
			delete m_pDrive;
			m_pDrive = nullptr;
		}
	}

	void SetHP(int Inhp = 100);//HP表示用UIの設定

	void DeleteHP();//HP表示用UIの削除

	void SetMP();//MP表示用UIの設定

	void Damage(int dmg);//被ダメージ演出と計算

	void Finish();//消滅時エフェクト
};
