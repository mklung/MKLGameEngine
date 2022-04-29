#pragma once

#include "EditMesh.h"
#include "RenderGpuBuffer.h"
#include "Vertex/VertexLayoutManager.h"

namespace sge {

	class RenderMesh {
	public:
		void create(const EditMesh& src);


		size_t vertexCount() const { return _vertexCount; }
		RenderGpuBuffer* vertexBuf() { return _vertexBuf; }
		const VertexLayout* vertexLayout() const { return _vertexLayout; }

	private:


		size_t _vertexCount = 0;
		const VertexLayout* _vertexLayout = nullptr;
		RenderGpuBuffer*	_vertexBuf;
	};

}