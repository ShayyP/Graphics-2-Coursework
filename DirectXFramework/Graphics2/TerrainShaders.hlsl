cbuffer ConstantBuffer
{
    float4x4 completeTransformation;
    float4x4 worldTransformation;    
	float4 cameraPosition;
    float4 lightVector;			// the light's vector
    float4 lightColor;			// the light's color
    float4 ambientColor;		// the ambient light's color
    float4 diffuseCoefficient;	// The diffuse reflection cooefficient
	float4 specularCoefficient;	// The specular reflection cooefficient
	float  shininess;			// The shininess factor
	float  opacity;				// The opacity (transparency) of the material. 0 = fully transparent, 1 = fully opaque
	float2 padding;
}

Texture2D BlendMap : register(t0);
Texture2DArray TexturesArray : register(t1);
Texture2D WaterNormals : register(t2);

SamplerState ss
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexShaderInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
	float2 BlendMapTexCoord : TEXCOORD1;
	float2 WaterTexCoord : TEXCOORD2;
};

struct PixelShaderInput
{
	float4 Position : SV_POSITION;
	float4 PositionWS: TEXCOORD2;
	float4 NormalWS : TEXCOORD3;
	float2 TexCoord : TEXCOORD0;
	float2 BlendMapTexCoord : TEXCOORD1;
	float2 WaterTexCoord : TEXCOORD4;
};

PixelShaderInput VShader(VertexShaderInput vin)
{
    PixelShaderInput output;
	float3 position = vin.Position;
	output.Position = mul(completeTransformation, float4(position, 1.0f));
	output.PositionWS = mul(worldTransformation, float4(position, 1.0f));
	output.NormalWS = float4(mul((float3x3)worldTransformation, vin.Normal), 1.0f);
	output.TexCoord = vin.TexCoord;
	output.BlendMapTexCoord = vin.BlendMapTexCoord;
	output.WaterTexCoord = vin.WaterTexCoord;
	return output;
}

float4 PShader(PixelShaderInput input) : SV_TARGET
{
	float4 directionToCamera = normalize(input.PositionWS - cameraPosition);
	float4 directionToLight = normalize(-lightVector);
	float surfaceShininess = shininess;
	float4 adjustedNormal = normalize(input.NormalWS);
	// Adjust normal and shininess for water
	if (input.PositionWS.y < 1024 * 0.03)
	{
		float4 waterSample = WaterNormals.Sample(ss, input.WaterTexCoord);
		adjustedNormal = float4(waterSample.r, waterSample.g, waterSample.b, 0.0f);
		surfaceShininess = 10;
	}

	// Calculate diffuse lighting
	float NdotL = max(0, dot(adjustedNormal, directionToLight));
	float4 diffuse = saturate(lightColor * NdotL * diffuseCoefficient);
	diffuse.a = 1.0f;

	// Calculate specular component
	float4 R = 2 * NdotL * adjustedNormal - directionToLight;
	float RdotV = max(0, dot(R, directionToCamera));
	float4 specular = saturate(lightColor * pow(RdotV, surfaceShininess) * specularCoefficient);
	specular.a = 1.0f;

	// Calculate ambient lighting
	float4 ambientLight = ambientColor * diffuseCoefficient;
	float4 color;

	// Sample layers in texture array.
	float4 c0 = TexturesArray.Sample(ss, float3(input.TexCoord, 0.0f));
	float4 c1 = TexturesArray.Sample(ss, float3(input.TexCoord, 1.0f));
	float4 c2 = TexturesArray.Sample(ss, float3(input.TexCoord, 2.0f));
	float4 c3 = TexturesArray.Sample(ss, float3(input.TexCoord, 3.0f));
	float4 c4 = TexturesArray.Sample(ss, float3(input.TexCoord, 4.0f));

	// Sample the blend map.
	float4 t = BlendMap.Sample(ss, input.BlendMapTexCoord);

	// Blend the layers on top of each other.
	color = c0;
	color = lerp(color, c1, t.r);
	color = lerp(color, c2, t.g);
	color = lerp(color, c3, t.b);
	color = lerp(color, c4, t.a);
	// Blend in water colour
	if (input.PositionWS.y < 1024 * 0.03)
	{
		color = lerp(color, normalize(float4(0, 151, 240, 100)), 0.5);
	}

	// Combine all components
	color = (ambientLight + diffuse) * color;
	color = saturate(color + specular);
	return color;
}