cbuffer LightCBuf
{
    float3 lightPos;
};

static const float3 materialColor = { 1.0f, 0.0f, 0.0f };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
// set attenuation
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    // fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    // directtion from light to object
    const float3 dirToL = vToL / distToL;
    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    // diffuse intensity (Book DirectX12 p.324)
    // if the normal directs opposition to dirToL , turn diffuse to 0
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    
    // final color
    return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
}