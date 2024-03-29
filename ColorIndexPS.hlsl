cbuffer CBuf
{
	float4 face_colors[8];
};

// SV_PrimitiveID will tell pipeline to generate a primitive ID for every triangle
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return face_colors[(tid/2) % 8];
}