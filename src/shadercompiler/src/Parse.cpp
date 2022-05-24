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
		token.type = TokenType::Identifier;
		while (token.type != TokenType::None)
		{
			token = lexer.nextToken();
		} 
		

	}

	void ShaderContent()
	{

	}

	void PassContent()
	{

	}
}