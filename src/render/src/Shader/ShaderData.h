#pragma once

#include "sge_core.h"
#include "RenderDataType.h"
#include "Vertex/Vertex.h"
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



	struct ShaderInfo
	{

		struct Pass
		{
		public:
			String name;
			String vsEntryPt;
			String psEntryPt;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, vsEntryPt);
				SGE_NAMED_IO(se, psEntryPt);
			}
		};

		struct  Prop
		{
		public:
			ShaderPropType propTypr = ShaderPropType::None;
			String		name;
			String		displayName;
			String		defaultValue;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, displayName);
				SGE_NAMED_IO(se, defaultValue);
			}
		};

	public:
		String path;
		String fileName;
		String shaderName;
		Vector_<Prop, 8> props;
		Vector_<Pass, 2> passes;

		void clear();

		template<class SE>
		void onJson(SE& se) {
			SGE_NAMED_IO(se, path);
			SGE_NAMED_IO(se, fileName);
			SGE_NAMED_IO(se, shaderName);
			SGE_NAMED_IO(se, props);
			SGE_NAMED_IO(se, passes);
		}
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

		bool CheckToken(TokenType _type, String _expectValue) {return (type == _type && value == _expectValue);}
		bool CheckToken(TokenType _type) {return (type == _type);}
	};







	class  ShaderDescData
	{
	public:

		class  ShaderInputParam
		{
		public:
			String			name;
			String			attrId;
			RenderDataType dataType = RenderDataType::None;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, attrId);
				//SGE_NAMED_IO(se, dataType);
			}
		};

		class  ShaderVariable
		{
		public:
			String			name;
			int				offset = 0;
			RenderDataType	dataType = RenderDataType::None;

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, offset);
				SGE_NAMED_IO(se, dataType);
			}

		};
				
		class  ConstBufferInfo
		{
		public:
			String	name;
			int		bindPoint = 0;
			int		bindCount = 0;
			size_t	dataSize = 0;
			Vector_<ShaderVariable, 4> variables;

			const ShaderVariable* findVariable(StrView propname) const
			{
				for (auto& v : variables)
				{
					if (v.name == propname) return &v;
				}

				return nullptr;
			}

			template<class SE>
			void onJson(SE& se) {
				SGE_NAMED_IO(se, name);
				SGE_NAMED_IO(se, bindPoint);
				SGE_NAMED_IO(se, bindCount);
				SGE_NAMED_IO(se, dataSize);
				SGE_NAMED_IO(se, variables);
			}

		};

		String profile;
		Vector_<ShaderInputParam, 8> inputs;
		Vector_<ConstBufferInfo, 8> constBuffers;

		void Clear();

		template<class SE>
		void onJson(SE& se) {
			SGE_NAMED_IO(se, profile);
			SGE_NAMED_IO(se, inputs);
			SGE_NAMED_IO(se, constBuffers);
		}
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


}