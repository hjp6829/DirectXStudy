struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

float4 main(VSOut IN) : SV_TARGET
{
    float4 texColor = tex.Sample(splr, IN.uv);
    if (texColor.r > 0.1)
    {
        return float4(0, 0, 0, 0);
    }
    float2 texelSize = float2(1.0f / 1280,1.0f / 960);
    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float2 offset = float2(x, y) * texelSize;
            float4 neighborColor = tex.Sample(splr, IN.uv + offset);
            if(neighborColor.r > 0.1)
            {
                return float4(1, 1, 1, 1);
            }
        }
    }

    return float4(0, 0, 0, 0);
}