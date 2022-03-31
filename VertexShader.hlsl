// four demensions vector because homogeneous coordinates
cbuffer CBuf
{
	matrix transform;
};

float4 main(float3 pos : Position) : SV_Position
{
	//multiple from left to right
	return mul(float4(pos, 1.0f),transform);
}