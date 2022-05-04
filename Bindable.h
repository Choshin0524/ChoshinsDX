#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	// to pass pointers to Context and Device to children classes
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
};