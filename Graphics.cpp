#include "Graphics.h"
#include <wrl.h>
#include <sstream>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL;

namespace dx = DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

// check and throw
#define GFX_THROW_IF_FAILED(hrcall) if (FAILED(hr=(hrcall))) throw Graphics::Exception(__LINE__, __FILE__, (hr))
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // will be specified if interlaced
	// no anti-aliasing
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2; // using 2 back buffer (totally 3 buffers)
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	// for checking results of d3d functionss
	HRESULT hr;

	// create device and front/back buffers,and swap chain and rendering context
	GFX_THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap, // release and get address ( & is overloaded )
		&pDevice,
		nullptr,
		&pContext
	));
	// gain access to texture subresource in swap chain (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_IF_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_IF_FAILED(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		pTarget.GetAddressOf()
	));

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	// less Z value will be passed (which means closer to the screen)
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_IF_FAILED(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	
	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_IF_FAILED(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_IF_FAILED(pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	));

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftX = 0.0f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason()); // due to sth like a driver crash or overclocking
		}
		else
		{
			GFX_THROW_IF_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	// whenever clean the screen also clean Zbuffer
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noexcept
{
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

// Graphics Exception Stuff
Graphics::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	ChiliException(line, file), // constructor for base class
	hr(hr)
{}

const char* Graphics::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::GetType() const noexcept
{
	return "Graphics Exception";
}

std::string Graphics::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Graphics::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::Exception::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hr);
}
