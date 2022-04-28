#pragma once

#include "EditMesh.h"

namespace sge {

	class RenderMesh {
	public:
		void create(const EditMesh& src);
		size_t vertexCount() const { return _vertexCount; }
	private:
		size_t _vertexCount = 0;

		const VertexLayout* _vertexLayout = nullptr;
	};

}