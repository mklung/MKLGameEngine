
#include "Lexer.h"

namespace sge
{
	Lexer::Lexer(StrView src)
	{
		_src = src;
	}

	const String skipChar = " \n\t\r\0";
	const String operatorChar = ".+-*/{}()<>,;#:=<>|@$%&[]!\'";

	Token Lexer::nextToken()
	{
		
 		SkipChar();
		if (End()) { _token.type = TokenType::None; return _token; }

		auto token = _src.at(viewPos);
		
		
		if ((token >= 'a' && token <= 'z') ||
			(token >= 'A' && token <= 'Z') ||
			(token == '_'))
		{
			_token.type = TokenType::Identifier;
			int viewEnd = viewPos;

			while ((token >= 'a' && token <= 'z') ||
					(token >= 'A' && token <= 'Z') ||
					(token >= '0' && token <= '9') ||
					(token == '_'))
			{
				if (viewEnd > _src.size() - 1)
					break;

				viewEnd++;
				token = _src.at(viewEnd);
			}

			int viewSize = (viewEnd - viewPos);
			_token.value = _src.substr(viewPos, viewEnd - viewPos);
			viewPos = viewEnd;

			SGE_LOG("Idf\t{}\t: {}", _lineNumber, _token.value);

			return _token;
		}

		else if (token == '"') 
		{
			_token.type = TokenType::String;
			int viewEnd = viewPos + 1;
			token = _src.at(viewEnd);

			while (token  != '"')
			{
				if (viewEnd > _src.size() - 1)
					break;

				viewEnd++;
				token = _src.at(viewEnd);
			}
			
			if (viewPos + 1 != viewEnd)
			{
				int viewSize = (viewEnd - viewPos);
				_token.value = _src.substr(viewPos + 1, viewSize - 1);
			}
			else
				_token.value = "";


			viewPos = viewEnd + 1;
			SGE_LOG("String\t{}\t: {}", _lineNumber, _token.value);
			return _token;
				


		}

		else if (token >= '0' && token <= '9')
		{
			_token.type = TokenType::Number;
			int viewEnd = viewPos;

			while (token >= '0' && token <= '9' || token == '.')
			{
				if (viewEnd > _src.size() - 1)
					break;

				viewEnd++;
				token = _src.at(viewEnd);
			}

			int viewSize = (viewEnd - viewPos);
			_token.value = _src.substr(viewPos, viewEnd - viewPos);

			if (_token.value.back() == '.')
				_token.value += '0';

			viewPos = viewEnd;

			SGE_LOG("Number\t{}\t: {}", _lineNumber, _token.value);
			return _token;
		}


		else if	(CheckOperatorChar(token))
		{
			if (token == '/' && _src.at(viewPos + 1) == '/')
			{
				_token.type = TokenType::Comment;
				int viewEnd = viewPos;

				while (token != '\n')
				{
					if (viewEnd > _src.size() - 1)
						break;

					viewEnd++;
					token = _src.at(viewEnd);
				}


				
				int viewSize = (viewEnd - viewPos);
				_token.value = _src.substr(viewPos, viewEnd - viewPos);
				SGE_LOG("CM\t{}\t: {}", _lineNumber, _token.value);
				viewPos = viewEnd;
				return _token;
			}

			_token.type = TokenType::Operator;
			_token.value = token;

			viewPos++;
			SGE_LOG("Opt\t{}\t: {}", _lineNumber, _token.value);
			return _token;
		}

		_token.type = TokenType::None;
		_token.value = token + "undefination";
		SGE_LOG("Opt\t{}\t: {}", _lineNumber, _token.value);
		return _token;
		
	}

	void Lexer::SkipChar()
	{
		if (viewPos >= _src.size() - 1)
			return;

		auto token = _src.at(viewPos);
		while (token == ' ' || token == '\n' || token == '\t' || token == '\r')
		{
			if (token == '\n')
				_lineNumber++;

			if (viewPos > _src.size() - 1)
				break;

			viewPos++;
			token = _src.at(viewPos);
		}

	}
	bool Lexer::CheckOperatorChar(char _t)
	{
		for (size_t i = 0; i < operatorChar.size(); i++)
		{
			if(_t == operatorChar[i])
				return true;
		}
		return false;
	}

	bool Lexer::End()
	{
		return viewPos > _src.size() - 1? true : false;
	}

}