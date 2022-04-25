#pragma once
#include <d3d11.h>


namespace sge {

	class Renderer_DX11
	{

	public:
		Renderer_DX11();
		Renderer_DX11(const Renderer_DX11&) = delete;
		Renderer_DX11& operator = (const Renderer_DX11&) = delete;
		~Renderer_DX11();

		struct VERTEX { FLOAT X, Y, Z; FLOAT Color[4]; };
		void InitD3D(HWND hwnd);     // sets up and initializes Direct3D
		void CleanD3D(void);         // closes Direct3D and releases memory
		void RenderFrame(void);     // renders a single frame
		void InitGraphics(void);    // creates the shape to render
		void InitPipeline(void);    // loads and prepares the shaders

	private:
		IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
		ID3D11Device* dev;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView* backbuffer;    // global declaration
		ID3D11InputLayout* pLayout;            // the pointer to the input layout
		ID3D11VertexShader* pVS;               // the pointer to the vertex shader
		ID3D11PixelShader* pPS;                // the pointer to the pixel shader
		ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

	};

}
