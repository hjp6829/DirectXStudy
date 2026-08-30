cbuffer CBuf1 : register(b0)
{
    float4 color[6];
};
cbuffer CBuf2 : register(b1)
{
    float4 lightPos;
    float4 lightColor;
    float3 cameraPos;
    float padding;
};

float4 main() : SV_TARGET
{
    return float4(lightColor.x, lightColor.y, lightColor.z, 1.0f);
}