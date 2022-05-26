#pragma once

#include "sge_core.h"

namespace sge
{
	class Pass
	{
	public:
		String vsEntryPt;
		String psEntryPt;
	};

	class ShaderData : NonCopyable
	{
	public:
		String shaderPath;
		String shaderName;
		Vector<Pass> pass;
	private:
	};

	enum class TokenType
	{
		None,
		Identifier,
		Operator,
		Number,
		String,
		Comment,
	};

	struct Token
	{
		TokenType type;
		String value;

	};
}