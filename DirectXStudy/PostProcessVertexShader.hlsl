struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};
//다 그리고 난 후에 오브젝트의 외각선을 그려야 하니까 새롭게 버텍스랑 픽셀을 사용해야함
VSOut main(uint id : SV_VertexID)
{
    VSOut output;

    float2 pos[3] =
    {
        float2(-1.0, -1.0),
        float2(-1.0, 3.0),
        float2(3.0, -1.0)
    };

    float2 uv[3] =
    {
        float2(0.0, 1.0),
        float2(0.0, -1.0),
        float2(2.0, 1.0)
    };

    output.pos = float4(pos[id], 0.0, 1.0);
    output.uv = uv[id];

    return output;
}