cbuffer ConstantBuffer
{
	matrix worldViewProj;	// The complete transformation
};


struct VertexIn
{
	float3 Position : POSITION;
	float4 Colour : COLOUR;
};

struct VertexOut
{
	float4 Position  : SV_POSITION;
	float4 Colour	 : COLOUR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	// Transform to homogeneous clip space.
	vout.Position = mul(worldViewProj, float4(vin.Position, 1.0f));
	vout.Colour = vin.Colour;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Colour;
}