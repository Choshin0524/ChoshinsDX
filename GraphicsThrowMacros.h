// check and throw
#define GFX_THROW_IF_FAILED(hrcall) if (FAILED(hr=(hrcall))) throw Graphics::Exception(__LINE__, __FILE__, (hr))
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define INFOMAN(gfx) HRESULT hr