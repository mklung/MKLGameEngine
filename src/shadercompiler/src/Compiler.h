#pragma once

#include "Lexer.h"
#include "ShaderData.h"
#include "DX11/RenderCommon_DX11.h"

namespace sge
{
	class ShaderCompiler
	{
	public:

		void CompilerShader(ShaderData* shaderData);
	private:
		void ShaderReflect(ComPtr<ID3DBlob>& bytecode);
	};

}