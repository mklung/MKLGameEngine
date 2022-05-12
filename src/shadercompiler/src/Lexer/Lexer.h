
#pragma once
#include "sge_core.h"
namespace sge
{
	enum  class TokenType
	{
		None,
		Identifier,
		Operator,
		Number,
		String,
	};
	struct Token
	{
		TokenType type;
		String value;

	};

	class Lexer : public NonCopyable
	{
	public:

		static void lexer(Span<const u8> src);
	private:

		void _lexer(Span<const u8> src);
		void _nextLine();
		void _nextChar(char* c, int* index);
		void _nextToken();
		void _parseLine();

		void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

		StrView _src;
		StrView _token;
		StrView _currentLine;
		StrView _lineRemain;
		StrView _srcRemain;

		int _lineNumber = 0;

		Vector<Token> _tokens;

	};
}