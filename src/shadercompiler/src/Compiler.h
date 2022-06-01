#pragma once

#include "Lexer.h"
#include "DX11/RenderCommon_DX11.h"

namespace sge
{
	const String COMPILE_FILE_PATH = "LocalTemp/Shader/DX11";

	class ShaderCompiler
	{
		using D11_PARAM_DESC = D3D11_SIGNATURE_PARAMETER_DESC;

	public:

		void CompilerShader(ShaderData* shaderData);
	private:
		void ShaderReflect(ComPtr<ID3DBlob>& bytecode, String profile, String fileName);
		RenderDataType ConvertShaderDataType(D11_PARAM_DESC* desc);

	};

}