struct VSOut
{
    float4 clipPos  : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 worldPos : Position;
    float3 Tangent : TEXCOORD0;
    float3 Bitangent : TEXCOORD1;
};

cbuffer CBuf
{
    matrix worldMatrix;
    matrix worldViewProjectionMatrix;
};

VSOut main(float3 localPos  : POSITION, float2 uv : UV, float3 normal : NORMAL, float3 tangent : Tangent)
{
    VSOut OUT;
    OUT.clipPos  = mul(float4(localPos , 1), worldViewProjectionMatrix);
    OUT.worldPos = mul(float4(localPos , 1), worldMatrix);
    OUT.uv = uv;
    OUT.normal = normalize(mul(normal, (float3x3) worldMatrix));
    OUT.Tangent = normalize(mul(tangent, (float3x3) worldMatrix));
    OUT.Bitangent = normalize(cross(OUT.normal, OUT.Tangent));
    return OUT;
}