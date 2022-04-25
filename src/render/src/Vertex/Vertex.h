#pragma once

namespace sge {

	enum class VertexType : u64 { None };

	enum class RenderDataType : u8 {
		None,

		Int8,	Int8x2,		Int8x3,		Int8x4,
		Int16,	Int16x2,	Int16x3,	Int16x4,
		Int32,	Int32x2,	Int32x3,	Int32x4,
		Int64,	Int64x2,	Int64x3,	Int64x4,

		UInt8,	UInt8x2,	UInt8x3,	UInt8x4,
		UInt16, UInt16x2,	UInt16x3,	UInt16x4,
		UInt32, UInt32x2,	UInt32x3,	UInt32x4,
		UInt64, UInt64x2,	UInt64x3,	UInt64x4,

		Float16, Float16x2, Float16x3, Float16x4,
		Float32, Float32x2, Float32x3, Float32x4,
		Float64, Float64x2, Float64x3, Float64x4,

		UNorm8, UNorm8x2, UNorm8x3, UNorm8x4,

	};


	enum class Vertex_SemanticType : u8 {
		None,
		Pos,
		Color,
		TexCoord,
		Normal,
		Tangent,
		Binormal,
	};

	enum class Vertex_Semantic : u16
	{
		None	= 0,
		Pos		= (enumInt(Vertex_SemanticType::Pos) << 8) | 0,
		Color0	= (enumInt(Vertex_SemanticType::Color) << 8) | 0,
		Color1	= (enumInt(Vertex_SemanticType::Color) << 8) | 1,
		Color2	= (enumInt(Vertex_SemanticType::Color) << 8) | 2,
		Color3	= (enumInt(Vertex_SemanticType::Color) << 8) | 3,

		TexCoord0 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 0,
		TexCoord1 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 1,
		TexCoord2 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 2,
		TexCoord3 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 3,
		TexCoord4 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 4,
		TexCoord5 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 5,
		TexCoord6 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 6,
		TexCoord7 = (enumInt(Vertex_SemanticType::TexCoord) << 8) | 7,

		Normal = (enumInt(Vertex_SemanticType::Normal) << 8) | 0,
		Tangent = (enumInt(Vertex_SemanticType::Tangent) << 8) | 0,
		Binormal = (enumInt(Vertex_SemanticType::Binormal) << 8) | 0,
	};

	struct VertexLayout : public NonCopyable
	{
		using Semantic = Vertex_Semantic;
		using DataType = RenderDataType;

		struct Element {
			Semantic	semantic = Semantic::None;
			u16			offset = 0;
			DataType	dataType = DataType::None;
		};

		VertexType type = VertexType::None;
		size_t	stride = 0;
		Vector_<Element, 16>	elements;

		template<class V, class A>
		void addElement(Semantic semantic, A V::* attr, size_t index = 0)
		{
			if (std::is_array<A>()) {
				size_t n = std::extent<A>();
				for (size_t i = 0; i < n; i++) {
					_addElement(semantic + static_cast<int>(i), attr, i);
				}
			}
			else {
				_addElement(semantic, attr, 0);
			}
		}

	private:
		template<class V, class A>
		void _addElement(Semantic semantic, A V::* attr, size_t index) {
			auto& o = elements.push_back();
			o.semantic = semantic;
			using A = std::remove_extent<A>::type;
			o.dataType = RenderDataTypeUtil::get<A>();
			o.offset = static_cast<u16>(memberOffset(attr) + sizeof(A) * index);
		}
	};
}