#include "VertexShader.h"
#include "GraphicsThrowMacros.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);
	GFX_THROW_IF_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pBlob)); // need wide str
	GFX_THROW_IF_FAILED(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	// Bind pixel shader to pipeline
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept	
{
	return pBlob.Get();
}
