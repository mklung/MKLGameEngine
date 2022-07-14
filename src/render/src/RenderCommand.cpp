#include "RenderCommand.h"
#include "Mesh/RenderMesh.h"

namespace sge {
	void RenderCommandBuffer::drawMesh(const SrcLoc& debugLoc, RenderMesh& mesh, Material* mat)
	{
		for (auto& sm : mesh.subMeshes()) {
			drawSubMesh(debugLoc, sm, mat);
		}

	}
	void RenderCommandBuffer::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh, Material* mat)
	{
		if (!mat) { SGE_ASSERT(false); return; }

		auto passes = mat->passes();

		for (size_t i = 0; i < passes.size(); i++)
		{
			auto* cmd = newCommand<RenderCommand_DrawCall>();

			cmd->material		= mat;
			cmd->materialPassIndex = i;

			cmd->primitive		= subMesh.primitive();
			cmd->vertexLayout	= subMesh.vertexLayout();
			cmd->vertexBuffer	= subMesh.vertexBuffer();
			cmd->vertexCount	= subMesh.vertexCount();
			cmd->indexBuffer	= subMesh.indexBuffer();
			cmd->indexType		= subMesh.indexType();
			cmd->indexCount		= subMesh.indexCount();
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
