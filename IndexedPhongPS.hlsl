cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
// set attenuation
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float3 materialColor[6];
    // gpu side : float float float padding * 5
    //            float float float NOPADDING -> next var
    float padding;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 worldPos : Position, float3 n : Normal, uint tid : SV_PrimitiveID) : SV_TARGET
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
    
    //reflected light vector(Check 3DFund specular lighting)
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    //calculate specular intensity
    const float3 specular =
    att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
                              
    
    // final color
    return float4(saturate(diffuse + ambient + specular) * materialColor[tid % 6], 1.0f);
}