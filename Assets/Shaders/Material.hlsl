struct VS_INPUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

	//WORLD SPACE
    output.position = mul(input.position, m_world);
    // VIEW SPACE
    output.position = mul(output.position, m_view);
    // SCREEN SPACE
    output.position = mul(output.position, m_proj);
    
    output.texcoord = input.texcoord;

    return output;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return Color.Sample(ColorSampler, input.texcoord);
}