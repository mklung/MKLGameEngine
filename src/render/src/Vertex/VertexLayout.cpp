#include "VertexLayout.h"

namespace sge {
	VertexLayoutManager* VertexLayoutManager::s_current = nullptr;

	VertexLayoutManager::VertexLayoutManager() {
		registerLayout<Vertex_Pos>();
		registerLayout<Vertex_PosColor>();

	}

	VertexLayoutManager* VertexLayoutManager::current() {
		static VertexLayoutManager s;
		return &s;
	}

	const VertexLayout* VertexLayoutManager::getLayout(VertexType type) {
		auto it = _table.find(type);
		if (it == _table.end()) {
			return nullptr;
		}
		return &it->second;
	}

	VertexLayout* VertexLayoutManager::_createLayout(VertexType type) 
	{
		auto* p = getLayout(type);
		if (p) {
			SGE_ASSERT(false);
		}
		return &_table[type];
	}
}