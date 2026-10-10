struct VSOut
{
    float4 clipPos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 worldPos : Position;
    float3 Tangent : TEXCOORD0;
    float3 Bitangent : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

float4 main(VSOut IN) : SV_TARGET
{
    float4 texColor = tex.Sample(splr, IN.uv);
    return texColor;
}