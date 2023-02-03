//頂点シェーダー
#include "common.hlsl" //必ずインクルード
//
//in は入力されてくるデーター
//out は出力するデータ
//
Texture2D g_Texture : register(t0);
Texture2D g_NormalTexture : register(t1);
Texture2D g_DepthTexture : register(t2);

void main(in VS_IN In, out PS_EdgeInput Out)
{
	matrix wvp; //行列変数を作成
	wvp = mul(World, View); //wvp = ワールド行列＊カメラ行列
	wvp = mul(wvp, Projection); //wvp = wvp *プロジェクション行列
	In.Position = mul(In.Position, wvp); //変換結果を出力する

	float2 texSize;
	float level;
	//テクスチャーのサイズを取得する
	g_Texture.GetDimensions(0, texSize.x, texSize.y, level);
	Out.Position = In.Position;
	float2 tex = In.TexCoord;

	float offset = 1.0f;
	//法線
	{
		//真ん中のピクセル
		Out.tex0 = tex;

		//右上のピクセル
		Out.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);

		//上のピクセル
		Out.tex2.xy = tex + float2(0.0f, -offset / texSize.y);

		//左上のピクセル
		Out.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//右のピクセル
		Out.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

		//左のピクセル
		Out.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

		//右下のピクセル
		Out.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

		//下のピクセル
		Out.tex7.xy = tex + float2(0.0f, offset / texSize.y);

		//左下のピクセル
		Out.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);
	}

	//深度値
	{
		//深度値を取り出すときに使うUV座標
		offset = 1.0f;
		//右上のピクセル
		Out.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//上のピクセル
		Out.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//左上のピクセル
		Out.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//右のピクセル
		Out.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//左のピクセル
		Out.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//右下のピクセル
		Out.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//下のピクセル
		Out.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//左下のピクセル
		Out.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);
	}
}