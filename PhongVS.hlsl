cbuffer Cbuf
{
    matrix model;
    matrix modelViewProj;   
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal)
{
    VSOut vso;
    vso.worldPos = (float3)mul(float4(pos, 1.0f), model);
    // use float3x3 casting because translation doesnt need to be calculated
    vso.normal = mul(n, (float3x3) model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}