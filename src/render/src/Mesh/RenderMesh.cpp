#include "RenderMesh.h"


namespace sge
{
	struct RenderMesh_InternalHelper {
		template<class DST, class T> inline
			static void copyVertexData(DST* dst
				, size_t vertexCount
				//, const VertexLayout::Element& element
				, size_t stride
				, const T* src)
		{
			u8* p = dst + element.offset;

			for (size_t i = 0; i < vertexCount; i++) {
				*reinterpret_cast<T*>(p) = *src;
				src++;
				p += stride;
			}
		}

		static bool hasAttr(size_t arraySize, size_t vertexCount) {
			if (arraySize <= 0) return false;
			if (arraySize < vertexCount) {
				SGE_ASSERT(false);
				return false;
			}
			return true;
		}
	};

	void RenderMesh::create(const EditMesh& src)
	{

		u8 uvCount = 0;
		u8 colorCount = 0;
		u8 normalCount = 0;
		u8 tangentCount = 0;
		u8 binormalCount = 0;

		using Helper = RenderMesh_InternalHelper;

		_vertexCount = src.pos.size();

		if (_vertexCount <= 0)
			return;

		if (Helper::hasAttr(src.color.size(), _vertexCount)) colorCount = 1;
		if (Helper::hasAttr(src.normal.size(), _vertexCount)) normalCount = 1;
		if (Helper::hasAttr(src.tangent.size(), _vertexCount)) tangentCount = 1;
		if (Helper::hasAttr(src.binormal.size(), _vertexCount)) binormalCount = 1;

		for (u8 i = 0; i < EditMesh::kUvCountMax; i++) {
			if (Helper::hasAttr(src.uv[i].size(), _vertexCount)) uvCount = i + 1;
		}

		auto vertexType = VertexTypeUtil::make(
			RenderDataTypeUtil::get<Tuple3f>(),
			RenderDataTypeUtil::get<Color4b>(), colorCount,
			RenderDataTypeUtil::get<Tuple2f>(), uvCount,
			RenderDataTypeUtil::get<Tuple3f>(), normalCount, tangentCount, binormalCount);


	}



}

