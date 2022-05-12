#include "Lexer.h"

namespace sge

{
	void Lexer::lexer(Span<const u8> src)
	{
		Lexer l;
		l._lexer(src);
	}

	void Lexer::_lexer(Span<const u8> src)
	{
		_src = StrView(reinterpret_cast<const char*>(src.data()), src.size());
		_srcRemain = _src;
		_lineNumber = 0;

		while (_srcRemain.size() > 0) {
			_nextLine();
			if (_currentLine.size()) 
			{
				_lineRemain = _currentLine;
				//_nextToken();
				_parseLine();
			}
		}
	}

	void Lexer::_nextLine()
	{
		auto ret = StringUtil::splitByChar(_srcRemain, "\n");
		_currentLine = ret.first;
		_srcRemain = ret.second;
		_lineNumber++;

		
		
	}



	void Lexer::_nextToken()
	{
		while (_lineRemain.size() >0 )
		{
			const char* c = _lineRemain.begin();
			String splitChar = "";

			if(*c != '"')
				splitChar = " \t\r";
			else
				splitChar = "\t\r";

			auto ret = StringUtil::splitByChar(_lineRemain, splitChar);

			_token = StringUtil::trimChar(ret.first, " \t\r");
			_lineRemain = StringUtil::trimChar(ret.second, " \t\r");
			SGE_LOG("T {}: {}", _lineNumber, _token);

		}

	}

	void Lexer::_parseLine()
	{
		int viewIndex = 0;
		char token;
		const char* src_c;
		const char* startPoint;
		Token t;


		while (viewIndex < _lineRemain.size())
		{
			token = _lineRemain[viewIndex];
			startPoint = _lineRemain.begin();
			src_c = startPoint + viewIndex;

			if (token == '\n') 
			{
				return;
			}


			else if (	(token >= 'a' && token <= 'z') || 
						(token >= 'A' && token <= 'Z') || 
						(token == '_')) 
			{
				t.type = TokenType::Identifier;
				String idf;
				char c;
				while (	(*src_c >= 'a' && *src_c <= 'z') || 
						(*src_c >= 'A' && *src_c <= 'Z') || 
						(*src_c >= '0' && *src_c <= '9') || 
						(*src_c == '_')) 
				{
					idf += *src_c;
					viewIndex++;
					src_c = startPoint + viewIndex;

					if (viewIndex >= _lineRemain.size())
						return;
				}
				t.value = idf;
				SGE_LOG("Identifier{} : {}", _lineNumber, t.value);
				
			}


			else if (token == '"') 
			{
				t.type = TokenType::String;
				String _s = "";
				viewIndex++;
				src_c = startPoint + viewIndex;

				while (*src_c != token) 
				{
					_s += *src_c;
					
					viewIndex++;
					src_c = startPoint + viewIndex;
					if (viewIndex >= _lineRemain.size())
						return;
				}

				
				t.value = _s;
				SGE_LOG("String{} : {}", _lineNumber, t.value);

			}

			else if (token == '/') 
			{
				SGE_LOG("Comments {}", _lineNumber);
				return;
			}
			else
			{
				viewIndex++;

			}
		}

	}

}