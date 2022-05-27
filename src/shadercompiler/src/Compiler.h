#pragma once

#include "Lexer.h"
#include "ShaderData.h"
#include "DX11/RenderCommon_DX11.h"
#include "RenderDataType.h"

namespace sge
{
	class ShaderCompiler
	{
		using D11_PARAM_DESC = D3D11_SIGNATURE_PARAMETER_DESC;

	public:

		void CompilerShader(ShaderData* shaderData);
	private:
		void ShaderReflect(ComPtr<ID3DBlob>& bytecode);
		RenderDataType ConvertShaderDataType(D11_PARAM_DESC* desc);
	};

}