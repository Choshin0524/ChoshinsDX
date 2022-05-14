#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	struct Transforms
	{
		// WHY REVERSED? supposed to be model -> modelVIewProj
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};
