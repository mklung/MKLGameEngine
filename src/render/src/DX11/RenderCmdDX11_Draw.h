#pragma once
#include <d3d11.h>
#include "sge_core/math/Tuple3.h"

namespace sge {

	using PrimitiveType = D3D_PRIMITIVE_TOPOLOGY;
	class Shader
	{
		ID3D11VertexShader* pVS;
		ID3D11PixelShader* pPS;
	};

	class RenderCmdDX11_Draw
	{
	public:
		
		PrimitiveType primitiveType;
		ID3D11InputLayout* pLayout;
		ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer
		ID3D11Buffer* pIndexBuffer;


		Shader* shader;
		int vertexCount;

		RenderCmdDX11_Draw();
		~RenderCmdDX11_Draw();

	private:

	};
}