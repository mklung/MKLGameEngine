
#pragma once

#include "ShaderData.h"

namespace sge
{


	class Lexer : public NonCopyable
	{
	public:
		Lexer(StrView src);
		StrView shaderFile;
		Token nextToken();
		bool End();

	private:
		void SkipChar();
		bool CheckOperatorChar(char t);

		void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

		int viewPos = 0;
		int endPos = 0;
		int _lineNumber = 1;

		StrView _src;

		Token _token;

	};
}