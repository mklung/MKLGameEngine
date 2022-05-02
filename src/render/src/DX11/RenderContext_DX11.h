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

		ID3D11InputLayout* _getTestInputLayout(const VertexLayout* src);
		VectorMap<const VertexLayout*, ID3D11InputLayout*> _testInputLayouts;


		virtual void onSetFrameBufferSize(Vec2f newSize);

		virtual void onBeginRender() override;
		virtual void onEndRender() override;
		virtual void onRender() override;

		void _createRenderTarget();
		void _setTestShaders();
		void _createVertex();


		virtual void onCommit(RenderCommandBuffer& cmdBuf);

	};
}