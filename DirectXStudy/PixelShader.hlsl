struct VSOut
{
    float4 clipPos  : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 worldPos : Position;
    float3 Tangent : TEXCOORD0;
    float3 Bitangent : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

Texture2D texNor : register(t1);
SamplerState splrNor : register(s1);

cbuffer CBuf1 : register(b0)
{
    float4 color;
};
cbuffer CBuf2 : register(b1)
{
    float4 lightPos;
    float4 lightColor;
    float4 cameraPos;
    float specularStrength;
    float shininess;
    float maxLightDistance;
    float padding2;
};
float4 main(VSOut IN) : SV_TARGET
{
    float3 normalMap = texNor.Sample(splrNor, IN.uv);
    normalMap = normalMap * 2.0 - 1.0;
    
    float3 normalTS = normalize(normalMap.x * IN.Tangent + normalMap.y * IN.Bitangent + normalMap.z * IN.normal);
    
    float3 N = normalize(IN.normal);
    float3 L = normalize(lightPos.xyz - IN.worldPos.xyz);
    float3 V = normalize(cameraPos.xyz - IN.worldPos.xyz);
    float3 H = normalize(L + V);
    float dis = distance(lightPos.xyz, IN.worldPos.xyz);
    float distanceRatio = saturate(dis / maxLightDistance);
    float attenuation = 1.0f - distanceRatio;
    
    float specular = pow(saturate(dot(normalTS, H)), shininess) * specularStrength;

    float diffuse = saturate(dot(normalTS, L));
    float brightness = saturate(0.5f + diffuse);

    float4 texColor = tex.Sample(splr, IN.uv);
    texColor.rgb *= brightness * lightColor.rgb * color.rgb;
    texColor.rgb += specular;
    texColor.rgb *= attenuation;
    return texColor;
    //return float4(1, 1, 1, 1);
}