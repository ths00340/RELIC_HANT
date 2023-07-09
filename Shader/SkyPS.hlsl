//ピクセルシェーダー
#include "common.hlsl" //必ずインクルード
#include "PainterTool.hlsl"
//
//in は入力されてくるデーター
//out は出力するデータ
//
void main(in PS_IN In, out PS_OUT Out)
{
	int Grid = 40;
	float StarSize = 0.05f;
	float Luminance = 1000.f;//光の強さ
	float3 SatellitePos = Light.Position;
	float4 TopColor, UnderColor, MoonColor;
	TopColor = float4(0.058f, 0.07f, 0.149f, 1.f);//夜float4(0.686f, 0.933f, 0.933f, 1.f);//
	UnderColor = float4(0.203f, 0.239f, 0.45f, 1.f);//夜float4(0.878f, 1.f, 1.f, 1.f);
	MoonColor = float4(0.968f, 0.972f, 0.941f, 1.f);
	Out.Normal = float4(1.f, 0.f, 0.f, 1.f);
	//float4(0.686f,0.933f,0.933f,1.f);  //昼
	//float4(0.878f,1.f,1.f,1.f);        //昼

	//描画したいピクセルのローカル座標を正規化
	float3 dir = normalize(In.WorldPosition);
	//ラジアンを算出する
	float2 rad = float2(atan2(dir.x, dir.z), asin(dir.y));
	float2 uv = rad / float2(3.141592f / 2.f, 3.141592f / 2.f);

	uv *= Grid; //格子状のマス目作成 UVにかけた数分だけ同じUVが繰り返し展開される

	float2 ist = floor(uv); //各マス目の起点
	float2 fst = frac(uv); //各マス目の起点からの描画したい位置

	float4 color = 0;

	//自身含む周囲のマスを探索
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			//周辺1×1のエリア
			float2 neighbor = float2(x, y);

			//点のxy座標
			float2 p = rand2(ist);

			//点と処理対象のピクセルとの距離ベクトル
			float2 diff = neighbor + p - fst;

			//色を星ごとにランダムに当てはめる
			float r = rand(p + 1);
			float g = rand(p + 2);
			float b = rand(p + 3);
			float4 randColor = float4(r, g, b, 1);

			//"点"と"現在描画しようとしているピクセルとの距離"を利用して星を描画するかどうかを計算
			//step(t,x) はxがtより大きい場合1を返す
			float interpolation = 1 - step(rand(p) * StarSize, length(diff));
			Out.Diffuse = lerp(color, randColor, interpolation);
		}
	}

	//整えたUVのY軸方向の座標を利用して色をグラデーションさせる
	Out.Diffuse += lerp(UnderColor, TopColor, dir.y + 0.75f);
	//月
	{
		float3 dir2 = normalize(SatellitePos); //太陽の位置ベクトル正規化
		float angle = dot(dir2, dir); //太陽の位置ベクトル　と　描画されるピクセルの位置ベクトル　の内積
		//_SunStrengthを大きくするほど計算結果は0に近づく
		float3 c = Out.Diffuse + (MoonColor * pow(max(0, angle), Luminance) * 1.f);
		Out.Diffuse.rgb = c;
	}
}