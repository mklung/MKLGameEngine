#include "RenderContext_DX11.h"
#include "Renderer_DX11.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh/EditMesh.h"

namespace sge
{

	RenderContext_DX11::RenderContext_DX11(CreateDesc & desc) : RenderContext(desc)
	{
		_renderer = Renderer_DX11::current();

		auto* win = static_cast<NativeUIWindow_Win32*>(desc.window);

		ID3D11Device1* dev = _renderer->d3dDevice();
		IDXGIFactory1* dxgiFactory = _renderer->dxgiFactory();


		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		{
			

			scd.BufferCount = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.OutputWindow = win->_hwnd;
			scd.SampleDesc.Count = 1;
			scd.Windowed = TRUE;

			dxgiFactory->CreateSwapChain(dev, &scd, &_swapChain);

		}
	}

	void RenderContext_DX11::onBeginRender()
	{
		if(!_renderTargetView)
			_renderTargetView->Release();
		_renderTargetView = nullptr;


		ID3D11DeviceContext4* ctx = _renderer->d3dDeviceContext();

		if (!_renderTargetView) {
			_createRenderTarget();
		}

		ID3D11RenderTargetView* rt = { _renderTargetView };
		//	ctx->OMSetRenderTargets(1, rt, _depthStencilView);
		ctx->OMSetRenderTargets(1, &rt, nullptr);

		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = 800;
		viewport.Height = 600;

		ctx->RSSetViewports(1, &viewport);
		_setTestShaders();
	}

	void RenderContext_DX11::onEndRender()
	{
	}

	
	void RenderContext_DX11::onRender()
	{
		

		auto* ctx = _renderer->d3dDeviceContext();

		float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		ctx->ClearRenderTargetView(_renderTargetView, color);


		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		ctx->IASetVertexBuffers(0, 1, &_testVertexBuffer, &stride, &offset);

		ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx->IASetInputLayout(_pLayout);

		ctx->Draw(3, 0);
		_swapChain->Present(0, 0);
	}

	void RenderContext_DX11::_createRenderTarget()
	{
		auto* renderer = Renderer_DX11::current();
		if (renderer == NULL)
			return;
		auto* dev = renderer->d3dDevice();

		ID3D11Texture2D* backBuffer;
		_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

		dev->CreateRenderTargetView(backBuffer, nullptr, &_renderTargetView);
		

		D3D11_TEXTURE2D_DESC backBufferDesc;
		backBuffer->GetDesc(&backBufferDesc);



		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = backBufferDesc.Width;
		descDepth.Height = backBufferDesc.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		dev->CreateTexture2D(&descDepth, nullptr, &_depthStencil);

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		dev->CreateDepthStencilView(_depthStencil, &descDSV, &_depthStencilView);
		backBuffer->Release();
	}

	void RenderContext_DX11::_setTestShaders()
	{
		ID3D11Device1*			dev = _renderer->d3dDevice();
		ID3D11DeviceContext4*	ctx = _renderer->d3dDeviceContext();


		ID3D10Blob* VS, * PS;
		D3DCompileFromFile(L"Shader/Triangle.hlsl", 0, 0, "vs_main", "vs_4_0", 0, 0, &VS, 0);
		D3DCompileFromFile(L"Shader/Triangle.hlsl", 0, 0, "ps_main", "ps_4_0", 0, 0, &PS, 0);
		dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &_testVertexShader);
		dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &_testPixelShader);

		ctx->VSSetShader(_testVertexShader, 0, 0);
		ctx->PSSetShader(_testPixelShader, 0, 0);

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &_pLayout);


		RenderContext_DX11::_createVertex();
	}

	void RenderContext_DX11::_createVertex()
	{
		ID3D11Device1* dev = _renderer->d3dDevice();
		ID3D11DeviceContext4* ctx = _renderer->d3dDeviceContext();

		VERTEX OurVertices[] =
		{
			{0.0f, 0.5f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}},
			{0.45f, -0.5, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f}},
			{-0.45f, -0.5f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}}
		};


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		dev->CreateBuffer(&bd, NULL, &_testVertexBuffer);       // create the buffer


		D3D11_MAPPED_SUBRESOURCE ms;
		ctx->Map(_testVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);		// map the buffer
		memcpy(ms.pData, OurVertices, sizeof(OurVertices));							// copy the data
		ctx->Unmap(_testVertexBuffer, NULL);										// unmap the buffer
	}

}