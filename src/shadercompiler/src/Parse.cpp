#pragma once

#include "Parse.h"
#include "Lexer.h"

namespace sge
{
	void ShaderParse::LoadFile(StrView filename)
	{
		MemMapFile mm;
		mm.open(filename);
		LoadMem(mm, filename);

	}
	void ShaderParse::LoadMem(Span<const u8> src, StrView filename)
	{
		ShaderParse sp;
		sp.SetshaderPath(filename);
		sp.Parse(src);

	}
	void ShaderParse::SetshaderPath(StrView filename)
	{
		String path = filename.data();
		_shaderData.shaderPath = path;
	}
	void ShaderParse::Parse(Span<const u8> src)
	{
		_src = StrView(reinterpret_cast<const char*>(src.data()), src.size());
		Lexer lexer(_src);
		Token token;

		
		while (true)
		{
			token = lexer.nextToken();
			
			if (token.type == TokenType::None)
				break;

			else if (token.type == TokenType::Identifier)
			{
				if (token.value == "Shader")
				{
					token = lexer.nextToken();
					if (token.type == TokenType::None) break;

					if (token.type == TokenType::String)
						_shaderData.shaderName = token.value;
				}
				else if (token.value == "Pass")
				{
					Pass newPass;
					while (token.value != "}")
					{
						token = lexer.nextToken();
						if (token.type == TokenType::None) break;

						if (token.type == TokenType::Identifier && token.value == "VsFunc")
						{
							token = lexer.nextToken();
							if (token.type == TokenType::None) break;

							else if (token.type == TokenType::Identifier)
								newPass.vsEntryPt = token.value;
						}
						else if (token.type == TokenType::Identifier && token.value == "PsFunc")
						{
							token = lexer.nextToken();
							if (token.type == TokenType::None) break;

							else if (token.type == TokenType::Identifier)
								newPass.psEntryPt = token.value;
						}
					}
					_shaderData.pass.emplace_back(newPass);
				}
			}
		} 
		

		SGE_LOG("{}", _shaderData.shaderName);

	}


}