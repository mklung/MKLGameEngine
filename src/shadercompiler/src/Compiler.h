#pragma once

#include "Lexer.h"

namespace sge
{
	class ShaderCompiler
	{
	public:
		void GetEntryPoint(Vector<Token>& t) { onGetEntryPoint(t); }
		//void compilerShader(Vector<Token> &t) { onCompilerShader(t); }

	private:
		void onGetEntryPoint(Vector<Token>& t);
		void onCompilerShader();
		String pShaderEntryPt;
		String vShaderEntryPt;
	};

}