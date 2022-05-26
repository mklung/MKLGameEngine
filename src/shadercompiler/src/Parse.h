#pragma once


#include "ShaderData.h"
#include "Lexer.h"
#include <Mesh/EditMesh.h>

namespace sge
{

	class ShaderParse
	{
	public:
		static void LoadFile(StrView filename);
		static void LoadMem(Span<const u8> src, StrView filename);

		void SetshaderPath (StrView filename);
		void Parse (Span<const u8> src);

		ShaderData* shaderData() { return &_shaderData; }
	private:
		ShaderData _shaderData;

		StrView _shaderText;
		StrView _src;
	};

	
}