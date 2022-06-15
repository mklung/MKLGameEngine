
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"
#include "RenderGpuBuffer_DX11.h"
#include "Material_DX11.h"

namespace sge {

	

Renderer_DX11::Renderer_DX11(CreateDesc& desc)
{
	D3D_FEATURE_LEVEL featureLevel;
	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	ID3D11Device*				d3dDevice;
	ID3D11DeviceContext*		d3dDeviceContext;

	D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&d3dDevice,
		&featureLevel,
		&d3dDeviceContext);

	d3dDevice->QueryInterface(IID_PPV_ARGS(_d3dDev.ptrForInit()));
	d3dDeviceContext->QueryInterface(IID_PPV_ARGS(_d3dCtx.ptrForInit()));
	_d3dDev->QueryInterface(IID_PPV_ARGS(_dxgiDev.ptrForInit()));

	IDXGIAdapter* adapter;
	_dxgiDev->GetAdapter(&adapter);
	adapter->QueryInterface(IID_PPV_ARGS(_dxgiAdapter.ptrForInit()));
	D3D11_FEATURE_DATA_THREADING s = {};
	d3dDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &s, sizeof(s));

	_dxgiAdapter->GetParent(IID_PPV_ARGS(_dxgiFactory.ptrForInit()));

}

RenderContext* Renderer_DX11::onCreateContext(RenderContext_CreateDesc& desc)
{
	return new RenderContext_DX11(desc);
}

RenderGpuBuffer* Renderer_DX11::onCreateGpuBuffer(RenderGpuBuffer_CreateDesc& desc)
{
	return new RenderGpuBuffer_DX11(desc);
}

SPtr<Shader> Renderer_DX11::onCreateShader(StrView filename)
{
	return new Shader_DX11(filename);
}


}