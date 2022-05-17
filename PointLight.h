#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept;
	void Bind(Graphics& gfx, DirectX::XMMATRIX view) const noexcept;
private:
	struct PointLightCbuf
	{
		// gpu side repects 16bytes for float3 so need 4bytes padding
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		// set attenuation
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCbuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCbuf> cbuf;
};