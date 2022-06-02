#pragma once

#include "Parse.h"
#include "Lexer.h"
#include "Compiler_DX11.h"

namespace sge
{
	void ShaderParse::LoadFile(StrView filename)
	{
		MemMapFile mm;
		mm.open(filename);

	}

	void ShaderParse::SetshaderPath(StrView filename)
	{
		String path = filename.data();
		int sPos = path.find_last_of("/\\");
		int ePos = path.find_last_of('.');
		int size = ePos - sPos;
		String fn = path.substr(sPos + 1, size - 1);

		_shaderData.fileName = fn;
		_shaderData.path = path;
	}
	void ShaderParse::Parse(Span<const u8> src)
	{
		_src = StrView(reinterpret_cast<const char*>(src.data()), src.size());
		Lexer lexer(_src);
		ShaderToken token;

		
		while (true)
		{
			token = lexer.nextToken();
			
			if (token.type == ShaderTokenType::None)
				break;

			else if (token.type == ShaderTokenType::Identifier)
			{
				if (token.value == "Shader")
				{
					token = lexer.nextToken();
					if (token.type == ShaderTokenType::None) break;

					if (token.type == ShaderTokenType::String)
						_shaderData.shaderName = token.value;
				}
				else if (token.value == "Pass")
				{
					ShaderPass newPass;
					while (token.value != "}")
					{
						token = lexer.nextToken();
						if (token.type == ShaderTokenType::None) break;

						if (token.type == ShaderTokenType::Identifier && token.value == "VsFunc")
						{
							token = lexer.nextToken();
							if (token.type == ShaderTokenType::None) break;

							else if (token.type == ShaderTokenType::Identifier)
								newPass.vsEntryPt = token.value;
						}
						else if (token.type == ShaderTokenType::Identifier && token.value == "PsFunc")
						{
							token = lexer.nextToken();
							if (token.type == ShaderTokenType::None) break;

							else if (token.type == ShaderTokenType::Identifier)
								newPass.psEntryPt = token.value;
						}
					}
					_shaderData.pass.emplace_back(newPass);
				}
			}
		} 
		
		SGE_LOG("\nFilePath:{}\nFileName:{}\nShaderName:{}\nPassSize:{}\n", _shaderData.path,
			_shaderData.fileName,
			_shaderData.shaderName,
			_shaderData.pass.size());

		ShaderCompiler hlslCompiler;
		hlslCompiler.CompilerShader(&_shaderData);

		//SGE_LOG("{}", _shaderData.shaderName);

	}


}