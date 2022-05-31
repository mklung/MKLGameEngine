#pragma once

#include "sge_core.h"
#include "RenderDataType.h"
#include "nlohmann/json.hpp"

namespace sge
{

	using Json = nlohmann::json;

	enum class ShaderPropType {
		None,
		Int,
		Float,
		Vec2f,
		Vec3f,
		Vec4f,
		Color4f,
	};


	class ShaderPass
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
		Vector<ShaderPass> pass;
	private:
	};

	enum class ShaderTokenType
	{
		None,
		Identifier,
		Operator,
		Number,
		String,
		Comment,
	};

	struct ShaderToken
	{
		ShaderTokenType type;
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
		u8 dataSize;
		Vector<ShaderVariable> variables;
		Json  ToJson();
	};

	struct  ShaderInputParam
	{
	public:
		String name;
		String attrId;
		RenderDataType dataType;

		Json  ToJson();
	};


	struct  ShaderDescData
	{
	public:
		String profile;
		Vector<ShaderInputParam> inputs;
		//Vector<ShaderVariable> params;
		Vector<ConstBufferDesc> constBuffers;

		Json ToJson();
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