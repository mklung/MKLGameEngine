#pragma once
#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>
#include "RenderContext.h"
#include "Renderer_DX11.h"

namespace sge 
{


	class RenderContext_DX11 : public RenderContext
	{
	public:

		RenderContext_DX11(CreateDesc& desc);
		struct VERTEX { FLOAT X, Y, Z; 
						FLOAT Color[4]; };

	protected:
		Renderer_DX11* _renderer = nullptr;

		IDXGISwapChain*			_swapChain;
		ID3D11RenderTargetView*	_renderTargetView;
		ID3D11Texture2D*		_depthStencil;
		ID3D11DepthStencilView*	_depthStencilView;

		ID3D11Buffer*			_testVertexBuffer;
		ID3D11VertexShader*		_testVertexShader;
		ID3DBlob*				_testVertexShaderBytecode;
		ID3D11PixelShader*		_testPixelShader;
		ID3D11InputLayout*		_pLayout;


		virtual void onBeginRender() override;
		virtual void onEndRender() override;
		virtual void onRender() override;

		void _createRenderTarget();
		void _setTestShaders();
		void _createVertex();

	};
}