#pragma once

#include "sge_core.h"
#include "RenderDataType.h"
#include "nlohmann/json.hpp"
#include "sge_core/serializer/json/JsonUtil.h"

namespace sge
{

	using Json = nlohmann::json;

	enum class ShaderDescMask {
		None,
		Vertex = 1 << 0,
		Pixel = 1 << 1,
	};

	enum class ShaderPropType {
		None,
		Int,
		Float,
		Vec2f,
		Vec3f,
		Vec4f,
		Color4f,
	};





	struct ShaderInfo
	{

		struct Pass
		{
		public:
			String name;
			String vsEntryPt;
			String psEntryPt;
		};

		struct  Prop
		{
		public:
			ShaderPropType propTypr = ShaderPropType::None;
			String		name;
			String		displayName;
			String		defaultValue;
		};

	public:
		String path;
		String fileName;
		String shaderName;
		Vector_<Prop, 8> props;
		Vector_<Pass, 1> passes;

		void clear();
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
	public:
		TokenType type;
		String value;

	};

	struct  ShaderVariable
	{
	public:
		String name;
		int offset;
		RenderDataType dataType;

		Json  ToJson();
	};

	struct  ConstBufferDesc
	{
	public:
		String name;
		int bindPoint;
		int bindCount;
		u8 dataSize;
		Vector_<ShaderVariable, 4> variables;
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
		Vector_<ShaderInputParam, 8> inputs;
		Vector_<ConstBufferDesc, 8> constBuffers;

		String ToJson();
		void Clear();
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