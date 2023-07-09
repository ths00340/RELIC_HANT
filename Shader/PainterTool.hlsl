#define PI 3.14159265359

//乱数
float rand(float2 uv)
{
	return frac(sin(dot(uv.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}

//乱数2つ
float2 rand2(float2 uv)
{
	float2 st = float2(dot(uv, float2(127.1, 311.7)),
		dot(uv, float2(269.5, 183.3)));

	return -1.0 + 2.0 * frac(sin(st) * 43758.5453123);
}

float2 rand2(float2 st, int seed)
{
	float2 s = float2(dot(st, float2(127.1, 311.7)) + seed, dot(st, float2(269.5, 183.3)) + seed);
	return -1.0 + 2.0 * frac(sin(s) * 43758.5453123);
}

float3 rand3(float3 p, int seed)
{
	float3 s = float3(dot(p, float3(127.1, 311.7, 74.7)) + seed,
		dot(p, float3(269.5, 183.3, 246.1)) + seed,
		dot(p, float3(113.5, 271.9, 124.6)) + seed);
	return -1.0 + 2.0 * frac(sin(s) * 43758.5453123);
}

float2 fract(float2 uv)
{
	float2 ret = float2(uv.x - floor(uv.x), uv.y - floor(uv.y));
	return ret;
}

//うねうね
float animation(float f, float time)
{
	float speed = 2.0;
	return sin(f * 6.283 + time * speed) * 0.5 + 0.5;
}

// 値を2つ受け取り2つ出力するバージョン
float2 animation2(float2 uv, float time)
{
	return float2(animation(uv.x, time), animation(uv.y, time));
}

float interpolation(float f)
{
	return f * f * f * (f * (6.0 * f - 15.0) + 10.0);;
}

float BlockNoise_(float2 uv, float time)
{
	uv *= 8.0;
	float2 i_uv = floor(uv);
	float o = animation(rand(i_uv), time);
	return o;
}

float Noise_(float2 uv, float time)
{
	uv *= 8.0;
	float2 i_uv = floor(uv);
	float2 f_uv = fract(uv);
	float f1 = animation(rand(i_uv + float2(0.0, 0.0)), time);
	float f2 = animation(rand(i_uv + float2(1.0, 0.0)), time);
	float f3 = animation(rand(i_uv + float2(0.0, 1.0)), time);
	float f4 = animation(rand(i_uv + float2(1.0, 1.0)), time);
	float o = lerp(
		lerp(f1, f2, interpolation(f_uv.x)),
		lerp(f3, f4, interpolation(f_uv.x)),
		interpolation(f_uv.y));
	return o;
}

//パーリンノイズ
float perlinNoise(float2 uv)
{
	float2 p = floor(uv);
	float2 f = frac(uv);
	float2 u = f * f * (3.f - 2.f * f);

	float v00 = rand(p + float2(0.f, 0.f));
	float v10 = rand(p + float2(1.f, 0.f));
	float v01 = rand(p + float2(0.f, 1.f));
	float v11 = rand(p + float2(1.f, 1.f));

	return  lerp(lerp(dot(v00, f - float2(0.f, 0.f)), dot(v10, f - float2(1.f, 0.f)), u.x),
		lerp(dot(v01, f - float2(0.f, 1.f)), dot(v11, f - float2(1.f, 1.f)), u.x), u.y) + 0.5f;
}

float perlinNoise(float2 uv, float time)
{
	float2 p = floor(uv);
	float2 f = frac(uv);
	float2 u = f * f * (3.f - 2.f * f);

	float v00 = animation2(rand2(p + float2(0.f, 0.f)), time);
	float v10 = animation2(rand2(p + float2(1.f, 0.f)), time);
	float v01 = animation2(rand2(p + float2(0.f, 1.f)), time);
	float v11 = animation2(rand2(p + float2(1.f, 1.f)), time);

	return  lerp(lerp(dot(v00, f - float2(0.f, 0.f)), dot(v10, f - float2(1.f, 0.f)), u.x),
		lerp(dot(v01, f - float2(0.f, 1.f)), dot(v11, f - float2(1.f, 1.f)), u.x), u.y) + 0.5f;
}

float noise(float2 st)
{
	// Splited integer and float values.
	float2 i = floor(st);
	float2 f = fract(st);

	float a = rand(i + float2(0.0, 0.0));
	float b = rand(i + float2(1.0, 0.0));
	float c = rand(i + float2(0.0, 1.0));
	float d = rand(i + float2(1.0, 1.0));

	// -2.0f^3 + 3.0f^2
	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float noise(float2 uv, float time)
{
	// Splited integer and float values.
	float2 i = floor(uv);
	float2 f = fract(uv);

	float a = animation(rand(i + float2(0.0, 0.0)), time);
	float b = animation(rand(i + float2(1.0, 0.0)), time);
	float c = animation(rand(i + float2(0.0, 1.0)), time);
	float d = animation(rand(i + float2(1.0, 1.0)), time);

	// -2.0f^3 + 3.0f^2
	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float noise2(float2 st, int seed)
{
	float2 p = floor(st);
	float2 f = frac(st);

	float w00 = dot(rand2(p, seed), f);
	float w10 = dot(rand2(p + float2(1.0, 0.0), seed), f - float2(1.0, 0.0));
	float w01 = dot(rand2(p + float2(0.0, 1.0), seed), f - float2(0.0, 1.0));
	float w11 = dot(rand2(p + float2(1.0, 1.0), seed), f - float2(1.0, 1.0));

	float2 u = f * f * (3.0 - 2.0 * f);

	return lerp(lerp(w00, w10, u.x), lerp(w01, w11, u.x), u.y);
}

float noise3(float3 st, int seed)
{
	float3 p = floor(st);
	float3 f = frac(st);

	float w000 = dot(rand3(p, seed), f);
	float w100 = dot(rand3(p + float3(1, 0, 0), seed), f - float3(1, 0, 0));
	float w010 = dot(rand3(p + float3(0, 1, 0), seed), f - float3(0, 1, 0));
	float w110 = dot(rand3(p + float3(1, 1, 0), seed), f - float3(1, 1, 0));
	float w001 = dot(rand3(p + float3(0, 0, 1), seed), f - float3(0, 0, 1));
	float w101 = dot(rand3(p + float3(1, 0, 1), seed), f - float3(1, 0, 1));
	float w011 = dot(rand3(p + float3(0, 1, 1), seed), f - float3(0, 1, 1));
	float w111 = dot(rand3(p + float3(1, 1, 1), seed), f - float3(1, 1, 1));

	float3 u = f * f * (3.0 - 2.0 * f);

	float r1 = lerp(lerp(w000, w100, u.x), lerp(w010, w110, u.x), u.y);
	float r2 = lerp(lerp(w001, w101, u.x), lerp(w011, w111, u.x), u.y);

	return lerp(r1, r2, u.z);
}

float fBm(float2 uv, float time)
{
	// Initial values
	float value = 0.0;
	float amplitude = .5;
	float frequency = 0.;
	float2 st = uv;
	//
	// Loop of octaves
	for (int i = 0; i < 5; i++) {
		value += amplitude * noise(st, time);
		st *= 2.;
		amplitude *= .5;
	}
	return value;
}

float fBm(float2 uv, float time, int octave)
{
	// Initial values
	float value = 0.0;
	float amplitude = .5;
	float frequency = 0.;
	float2 st = uv;
	//
	// Loop of octaves
	for (int i = 0; i < octave; i++) {
		value += amplitude * noise(st, time);
		st *= 2.;
		amplitude *= .5;
	}
	return value;
}

//円1
float disc(float2 uv, float size) {
	float d = distance(uv, float2(0.5, 0.5));
	float s = step(size, d);
	return s;
}

//輪っか
float4 ring(float2 uv, float radius, float width)
{
	float d = distance(0.5, uv);
	return -step(radius + width / 2, d) + step(radius - width / 2, d);
}

//円2
float Cycle(float2 uv, float size, float2 pos)
{
	float radius = size;
	float r = distance(uv, pos);
	return step(radius, r);
}

//アンチエイリアスな円
float SmoothCycle(float2 uv, float size, float2 pos)
{
	float radius = size;
	float r = distance(uv, pos);
	return smoothstep(radius, radius + 0.02, r);
}

//連続的な輪っか
float ContinueHoop(float2 uv, float2 pos, float width, float frequency)
{
	float len = distance(uv, pos);
	return step(1.f - width, sin(len * frequency));
}

//移動する連続的な輪っか
float MoveContinueHoop(float2 uv, float2 pos, float width, float frequency, float time)
{
	float len = distance(uv, pos) + time;
	return step(1.f - width, sin(len * frequency));
}

//入れたUVを分割
float2 SplitUV(float2 uv, float Split)
{
	return frac(uv * Split);
}

float grad_rot(float2 uv) {
	float dx = 0.5 - uv.x;
	float dy = 0.5 - uv.y;

	float rad = atan2(dx, -dy);
	rad = rad * 180 / PI + 180;

	float n = floor((_SinTime.x * 1000) / 360);
	float offset = _SinTime.x * 1000;
	rad = rad + n * 360;

	float d1 = distance(rad, offset) / 50;//現在
	float d2 = distance(rad, offset + 360) / 50;//追い越し
	float d3 = distance(rad, offset - 360) / 50;//遅れ

	return min(min(d1, d2), d3);//min関数：小さいほうの値を返す
}

float fan_shape(float2 uv, float angle) {
	float dx = 0.5f - uv.x;
	float dy = 0.5f - uv.y;

	float rad = atan2(dx, -dy);
	rad = rad * 180.f / PI + 180.f;

	float n1 = floor((_SinTime.x * 1000.f + angle) / (360.f));
	float n2 = floor((_SinTime.x * 1000.f) / (360.f));
	float offset1 = _SinTime.x * 1000.f - 360.f * n1;
	float offset2 = _SinTime.x * 1000.f - 360.f * n2;
	float s1 = step(rad, angle + offset1);
	float s2 = step(offset2, rad);

	return  (s1 * s2) * step(offset2, 360.f - angle) +
		(s1 + s2) * (1 - step(offset2, 360.f - angle))
		- disc(uv, 0.5f);
}

float grid(float2 st) {
	float r1 = -disc(st, 0.5f) + disc(st, 0.49f);
	float r2 = -disc(st, 0.3f) + disc(st, 0.29f);
	float r3 = -disc(st, 0.1f) + disc(st, 0.09f);
	return r1 + r2 + r3;
}

float fbm(float2 st, int seed) {
	float val = 0.0;
	float a = 0.5;

	for (int i = 0; i < 6; i++) {
		val += a * noise2(st, seed);
		st *= 2.0;
		a *= 0.5;
	}
	return val;
}

float fbm(float2 st, int seed, int octave) {
	float val = 0.0;
	float a = 0.5;

	for (int i = 0; i < octave; i++) {
		val += a * noise2(st, seed);
		st *= 2.0;
		a *= 0.5;
	}
	return val;
}

float3 DomainWarp(float2 st, float time)
{
	float3 mixColor1 = float3(0.8, 0.35, 0.12);
	float3 mixColor2 = float3(0.3, 0.75, 0.69);

	float3 color = float3(0.f, 0.745f, 0.9f);
	float2 q = float2(0.f, 0.f);
	q.x = fBm(st + float2(0.f, 0.f), time);
	q.y = fBm(st + float2(1.f, 0.f), time);

	float2 r = float2(0.0, 0.f);
	r.x = fBm(st + (4.0 * q) + float2(1.7, 9.2) + 0.15, 0);
	r.y = fBm(st + (4.0 * q) + float2(8.3, 2.8) + 0.12, 0);

	color = lerp(color, mixColor1, clamp(length(q), 0.0, 1.0));
	color = lerp(color, mixColor2, clamp(length(r), 0.0, 1.0));

	float f = fBm(st + 4.0 * r, 0);

	float coef = (f * f * f + (0.6 * f * f) + (0.5 * f));
	color *= coef;
	return color;
}

float3 Vec3Cross(float3 a, float3 b)
{
	float3 ret;
	float A, B;
	A = a.y * b.z;
	B = a.z * b.y;
	ret.x = A - B;

	A = a.z * b.x;
	B = a.x * b.z;
	ret.y = A - B;

	A = a.x * b.y;
	B = a.y * b.x;
	ret.z = A - B;

	return ret;
}

float ThresholdTransition(float value, float threshold, float lowValue, float highValue, float transitionWidth)
{
	float transitionStart = threshold - transitionWidth * 0.5f;
	float transitionEnd = threshold + transitionWidth * 0.5f;

	if (value < transitionStart)
	{
		return lowValue;
	}
	else if (value > transitionEnd)
	{
		return highValue;
	}
	else
	{
		float t = (value - transitionStart) / transitionWidth;
		return lowValue + t * (highValue - lowValue);
	}
}

float4 Transition(float value, float threshold1, float threshold2, float4 value1, float4 value2, float4 value3)
{
	if (value < threshold1)
	{
		return value1;
	}
	else if (value < threshold2)
	{
		float t = (value - threshold1) / (threshold2 - threshold1);
		return lerp(value1, value2, t);
	}
	else
	{
		float t = (value - threshold2) / (1.0 - threshold2);
		return lerp(value2, value3, t);
	}
}