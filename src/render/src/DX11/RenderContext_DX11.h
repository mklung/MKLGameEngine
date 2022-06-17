#pragma once
#include "RenderContext.h"
#include "RenderCommon_DX11.h"

namespace sge 
{


	class RenderContext_DX11 : public RenderContext
	{
	public:

		RenderContext_DX11(CreateDesc& desc);


		void onCmd_ClearFrameBuffers(RenderCommand_ClearFrameBuffers& cmd);
		void onCmd_SwapBuffers(RenderCommand_SwapBuffers& cmd);
		void onCmd_DrawCall(RenderCommand_DrawCall& cmd);

		Renderer_DX11* renderer() { return _renderer; }

		struct VERTEX { FLOAT X, Y, Z; 
						FLOAT Color[4]; };

	protected:
		Renderer_DX11* _renderer = nullptr;

		ComPtr<IDXGISwapChain>			_swapChain;
		ComPtr<ID3D11RenderTargetView>	_renderTargetView;
		ComPtr<ID3D11Texture2D>			_depthStencil;
		ComPtr<ID3D11DepthStencilView>	_depthStencilView;

		ComPtr<ID3D11Buffer>			_testVertexBuffer;
		ComPtr<ID3D11VertexShader>		_testVertexShader;
		ComPtr<ID3DBlob>				_testVertexShaderBytecode;
		ComPtr<ID3D11PixelShader>		_testPixelShader;
		ComPtr<ID3D11RasterizerState>	_testRasterizerState;

		ComPtr<ID3D11DepthStencilState>	_testDepthStencilState;
		ComPtr<ID3D11BlendState>		_testBlendState;
		//ComPtr<ID3D11InputLayout>		_pLayout;

		ID3D11InputLayout* _getTestInputLayout(const VertexLayout* src);
		VectorMap<const VertexLayout*, ComPtr<ID3D11InputLayout>> _testInputLayouts;


		virtual void onSetFrameBufferSize(Vec2f newSize);

		virtual void onBeginRender() override;
		virtual void onEndRender() override;

		void _createRenderTarget();
		void _setTestShaders(const VertexLayout* vertexLayout);


		virtual void onCommit(RenderCommandBuffer& cmdBuf);

	};
}