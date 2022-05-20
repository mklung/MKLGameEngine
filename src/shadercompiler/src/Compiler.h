#pragma once

#include "Lexer.h"

namespace sge
{
	class ShaderCompiler
	{
	public:
		void GetEntryPoint(Vector<Token>& t, StrView filename) { onGetEntryPoint(t, filename); }
		//void compilerShader(Vector<Token> &t) { onCompilerShader(t); }
		StrView loadedShader;
	private:
		void onGetEntryPoint(Vector<Token>& t, StrView filename);
		void onCompilerShader();
		StrView ShaderFileName;

		String pShaderEntryPt;
		String vShaderEntryPt;
	};

}