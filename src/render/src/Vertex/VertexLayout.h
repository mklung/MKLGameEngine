#include "Vertex.h"

namespace sge
{

	class VertexLayoutManager {
	public:
		VertexLayoutManager();

		static VertexLayoutManager* current();

		const VertexLayout* getLayout(VertexType type);

		VertexLayout* _createLayout(VertexType type);

		template<class VERTEX>
		void registerLayout() {
			VertexLayout* layout = _createLayout(VERTEX::kType);
			layout->stride = sizeof(VERTEX);
			layout->type = VERTEX::kType;
			VERTEX::onRegister(layout);
		}

	private:

		static VertexLayoutManager* s_current;
		Map<VertexType, VertexLayout>	_table;
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
		None = 0,
		Pos = (enumInt(Vertex_SemanticType::Pos) << 8) | 0,
		Color0 = (enumInt(Vertex_SemanticType::Color) << 8) | 0,
		Color1 = (enumInt(Vertex_SemanticType::Color) << 8) | 1,
		Color2 = (enumInt(Vertex_SemanticType::Color) << 8) | 2,
		Color3 = (enumInt(Vertex_SemanticType::Color) << 8) | 3,

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


	struct VertexBase {
		using Semantic = Vertex_Semantic;

		using PosType = void;
		using ColorType = void;
		using UvType = void;
		using NormalType = void;

		static const RenderDataType	kPosType = RenderDataType::None;
		static const RenderDataType	kColorType = RenderDataType::None;
		static const RenderDataType	kUvType = RenderDataType::None;
		static const RenderDataType	kNormalType = RenderDataType::None;

		static const u8 kColorCount = 0;
		static const u8 kUvCount = 0;
		static const u8 kNormalCount = 0;
		static const u8 kTangentCount = 0;
		static const u8 kBinormalCount = 0;

		static const VertexType kType = VertexType::None;
	};


#pragma region VertexLayout
	template<class POS_TYPE>
	struct VertexT_Pos : public VertexBase
	{
		using PosType = POS_TYPE;
		POS_TYPE pos;

		static const RenderDataType	kPosType = RenderDataTypeUtil::get<POS_TYPE>();
		static const VertexType kType = VertexTypeUtil::addPos(VertexType::None, kPosType);

		static void onRegister(VertexLayout* layout) {
			layout->addElement(Semantic::Pos, &VertexT_Pos::pos);
		}
	};

	template<class COLOR_TYPE, u8 COLOR_COUNT, class BASE>
	struct VertexT_Color : public BASE
	{
		using ColorType = COLOR_TYPE;
		COLOR_TYPE	color[COLOR_COUNT];

		static const RenderDataType	kColorType = RenderDataTypeUtil::get<COLOR_TYPE>();
		static const u8 kColorCount = COLOR_COUNT;
		static const VertexType kType = VertexTypeUtil::addColor(BASE::kType, kColorType, kColorCount);

		static void onRegister(VertexLayout* layout) {
			BASE::onRegister(layout);
			layout->addElement(Semantic::Color0, &VertexT_Color::color);
		}
	};

	template<class UV_TYPE, u8 UV_COUNT, class BASE>
	struct VertexT_Uv : public BASE
	{
		using UvType = UV_TYPE;
		UV_TYPE		uv[UV_COUNT];

		static const RenderDataType	kUvType = RenderDataTypeUtil::get<UV_TYPE>();
		static const u8 kUvCount = UV_COUNT;
		static const VertexType kType = VertexTypeUtil::addUv(BASE::kType, kUvType, kUvCount);

		static void onRegister(VertexLayout* layout) {
			BASE::onRegister(layout);
			layout->addElement(Semantic::TexCoord0, &VertexT_Uv::uv);
		}
	};

	template<class NORMAL_TYPE, u8 NORMAL_COUNT, class BASE>
	struct VertexT_Normal : public BASE
	{
		using NormalType = NORMAL_TYPE;
		NORMAL_TYPE	normal[NORMAL_COUNT];

		static const RenderDataType	kNormalType = RenderDataTypeUtil::get<NORMAL_TYPE>();
		static const u8 kNormalCount = NORMAL_COUNT;
		static const VertexType kType = VertexTypeUtil::addNormal(BASE::kType, kNormalType, kNormalCount);

		static void onRegister(VertexLayout* layout) {
			BASE::onRegister(layout);
			layout->addElement(Semantic::Normal, &VertexT_Normal::normal);
		}
	};

	template<class TANGENT_TYPE, u8 TANGENT_COUNT, class BASE>
	struct VertexT_Tangent : public BASE
	{
		using TangentType = TANGENT_TYPE;
		TANGENT_TYPE	tangent[TANGENT_COUNT];

		static const RenderDataType	kTangentType = RenderDataTypeUtil::get<TANGENT_TYPE>();
		static const u8 kTangentCount = TANGENT_COUNT;
		static const VertexType kType = VertexTypeUtil::addTangent(BASE::kType, kTangentCount);

		static void onRegister(VertexLayout* layout) {
			static_assert(std::is_same<TangentType, NormalType>::value, "");

			BASE::onRegister(layout);
			layout->addElement(Semantic::Tangent, &VertexT_Tangent::tangent);
		}
	};

	template<class BINORMAL_TYPE, u8 BINORMAL_COUNT, class BASE>
	struct VertexT_Binormal : public BASE
	{
		using BinormalType = BINORMAL_TYPE;
		BINORMAL_TYPE	binormal[BINORMAL_COUNT];

		static const RenderDataType	kBinormalType = RenderDataTypeUtil::get<BINORMAL_TYPE>();
		static const u8 kBinormalCount = BINORMAL_COUNT;
		static const VertexType kType = VertexTypeUtil::addBinormal(BASE::kType, kBinormalCount);

		static void onRegister(VertexLayout* layout) {
			static_assert(std::is_same<BinormalType, NormalType>::value, "");

			BASE::onRegister(layout);
			layout->addElement(Semantic::Binormal, &VertexT_Binormal::binormal);
		}
	};
#pragma endregion

	
	using Vertex_Pos = VertexT_Pos<Tuple3f>;
	using Vertex_PosColor = VertexT_Color<Color4b, 1, Vertex_Pos>;

	template<u8 UV_COUNT> 
	using Vertex_PosUv = VertexT_Uv<Tuple2f, UV_COUNT, Vertex_Pos>;
	template<u8 UV_COUNT> 
	using Vertex_PosColorUv = VertexT_Uv<Tuple2f, UV_COUNT, Vertex_PosColor>;

	template<u8 UV_COUNT> 
	using Vertex_PosNormalUv = VertexT_Normal<Tuple3f, 1, Vertex_PosUv<UV_COUNT>>;
	template<u8 UV_COUNT> 
	using Vertex_PosColorNormalUv = VertexT_Normal<Tuple3f, 1, Vertex_PosColorUv<UV_COUNT>>;

	template<u8 UV_COUNT> 
	using Vertex_PosTangentUv = VertexT_Tangent<Tuple3f, 1, Vertex_PosNormalUv<UV_COUNT>>;
	template<u8 UV_COUNT> 
	using Vertex_PosColorTangentUv = VertexT_Tangent<Tuple3f, 1, Vertex_PosColorNormalUv<UV_COUNT>>;

	template<u8 UV_COUNT> 
	using Vertex_PosBinormalUv = VertexT_Binormal<Tuple3f, 1, Vertex_PosTangentUv<UV_COUNT>>;
	template<u8 UV_COUNT> 
	using Vertex_PosColorBinormalUv = VertexT_Binormal<Tuple3f, 1, Vertex_PosColorTangentUv<UV_COUNT>>;
}