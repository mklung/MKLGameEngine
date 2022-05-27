
#pragma once

#include "ShaderData.h"

namespace sge
{


	class Lexer : public NonCopyable
	{
	public:
		Lexer(StrView src);
		StrView shaderFile;
		ShaderToken nextToken();
		bool End();

	private:
		void SkipChar();
		void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

		bool CheckOperatorChar(char t);

		int viewPos = 0;
		int endPos = 0;
		int _lineNumber = 1;
		bool debugLog = false;

		StrView _src;

		ShaderToken _token;

	};
}