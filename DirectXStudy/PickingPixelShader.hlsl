cbuffer CBuf1 : register(b0)
{
    int id;
    float3 padding;
};
uint4 main() : SV_TARGET
{
	return uint4(id, id, id,id);
}