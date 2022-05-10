#include "RenderCommand.h"
#include "Mesh/RenderMesh.h"
#include "RenderCommand.h"

namespace sge {
	void RenderCommandBuffer::drawMesh(const SrcLoc& debugLoc, RenderMesh& mesh)
	{
		for (auto& sm : mesh.subMeshes()) {
			drawSubMesh(debugLoc, sm);
		}

		/*auto* cmd = newCommand<RenderCommand_DrawCall>();

		cmd->primitive = mesh.primitive();
		cmd->vertexLayout = mesh.vertexLayout();
		cmd->vertexBuffer = mesh.vertexBuf();
		cmd->vertexCount = mesh.vertexCount();*/
	}
	void RenderCommandBuffer::drawSubMesh(const SrcLoc& debugLoc, const RenderSubMesh& subMesh)
	{
		auto* cmd = newCommand<RenderCommand_DrawCall>();

		cmd->primitive = subMesh.primitive();
		cmd->vertexLayout = subMesh.vertexLayout();
		cmd->vertexBuffer = subMesh.vertexBuffer();
		cmd->vertexCount = subMesh.vertexCount();
		cmd->indexBuffer = subMesh.indexBuffer();
		cmd->indexType = subMesh.indexType();
		cmd->indexCount = subMesh.indexCount();
	}
	void RenderCommandBuffer::reset()
	{
		_allocator.clear();
		for (auto* cmd : _commands) {
			cmd->~RenderCommand();
		}
		_commands.clear();
	}
}
