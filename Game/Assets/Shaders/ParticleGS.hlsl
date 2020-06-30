#include "Particle.hlsli"


static const uint vnum = 4;
static const float4 offset_array[vnum] =
{
	float4(-0.5, -0.5, 0, 0),    // ç∂â∫
	float4(-0.5, +0.5, 0, 0),    // ç∂è„
	float4(+0.5, -0.5, 0, 0),    // âEè„
	float4(+0.5, +0.5, 0, 0),    // âEâ∫

};


[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		element.Pos = input[0].Pos + offset_array[i] * 2.0;

		float4x4 rev = (float4x4)0;
		rev[0][0] = -1;
		rev[1][1] =  1;
		rev[2][2] = -1;
		rev[3][3] =  1;

		element.Pos = mul(element.Pos, matWorld);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);

		element.Color = input[0].Color;
		
		element.Tex = offset_array[i].xy;
		element.Tex.y *= -1;
		element.Tex += float2(0.5f,0.5f);

		output.Append(element);
	}
	output.RestartStrip();
}