#include "RenderCommand.h"
#include "Mesh/RenderMesh.h"

namespace sge {
	void RenderCommandBuffer::drawMesh(const SrcLoc& debugLoc, RenderMesh& mesh)
	{
		for (auto& sm : mesh.subMeshes()) {
			drawSubMesh(debugLoc, sm);
		}

	}
	void RenderCommandBuffer::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh)
	{
		//if (!mat) { SGE_ASSERT(false); return; }

		//for (auto&pass : mat->passes())
		{
			auto* cmd = newCommand<RenderCommand_DrawCall>();

			//cmd->materialPass	= pass.ptr();
			cmd->primitive = subMesh.primitive();
			cmd->vertexLayout = subMesh.vertexLayout();
			cmd->vertexBuffer = subMesh.vertexBuffer();
			cmd->vertexCount = subMesh.vertexCount();
			cmd->indexBuffer = subMesh.indexBuffer();
			cmd->indexType = subMesh.indexType();
			cmd->indexCount = subMesh.indexCount();
		}
		
	}
	void RenderCommandBuffer::reset()
	{
		for (auto* cmd : _commands) {
			cmd->~RenderCommand();
		}
		_commands.clear();
		_allocator.clear();
	}
}
