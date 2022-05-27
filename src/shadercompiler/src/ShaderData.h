#pragma once

#include "sge_core.h"

namespace sge
{
	enum class ShaderPropType {
		None,
		Int,
		Float,
		Vec2f,
		Vec3f,
		Vec4f,
		Color4f,
	};


	class Pass
	{
	public:
		String vsEntryPt;
		String psEntryPt;
	};

	class ShaderData : NonCopyable
	{
	public:
		String path;
		String fileName;
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

	struct  ShaderVariable
	{
		String name;
		int offset;
		ShaderPropType dataType;
	};

	struct  ConstBufferDesc
	{
	public:
		String name;
		int bindPoint;
		int bindCount;
		int dataSize;
		Vector<ShaderVariable> variables;
	};

	struct  ShaderInputParam
	{
	public:
		String name;
		String attrId;
		RenderDataType dataType;
	};


	struct  ShaderDesc
	{
		String profile;
		Vector<ShaderInputParam> inputs;
		//Vector<ShaderVariable> params;
		Vector<ConstBufferDesc> constBuffers;
	};


	inline
	bool enumTryParse(ShaderPropType& outValue, StrView str) 
	{
#define E(V) if (str == #V) { outValue = ShaderPropType::V; return true; }
		E(None)
			E(Int)
			E(Float)
			E(Vec2f)
			E(Vec3f)
			E(Vec4f)
			E(Color4f)
#undef E
			return false;
	}

	inline
	StrView enumStr(ShaderPropType v) 
	{
		switch (v) 
		{
#define E(V) case ShaderPropType::V: return #V;
			E(None)
				E(Int)
				E(Float)
				E(Vec2f)
				E(Vec3f)
				E(Vec4f)
				E(Color4f);
#undef E
		default: return "";
		}
	}
}