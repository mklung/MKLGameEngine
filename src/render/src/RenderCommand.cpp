#pragma once

#include "Vertex/Vertex.h"
#include "RenderGpuBuffer.h"
#include "Mesh/RenderMesh.h"
#include "sge_core.h"

namespace sge
{
	class RenderMesh;
	enum class RenderCommandType {
		None,
		ClearFrameBuffers,
		SwapBuffers,
		DrawCall,
	};

	class RenderCommand : NonCopyable {
	public:
		using Type = RenderCommandType;

		RenderCommand(Type type) : _type(type) {}

		virtual ~RenderCommand() {}

		Type type() const { return _type; }

	private:
		Type _type = Type::None;
	};

	class RenderCommand_DrawCall : public RenderCommand {
		using Base = RenderCommand;
	public:
		RenderCommand_DrawCall() : Base(Type::DrawCall) {}

		//RenderPrimitiveType		primitive = RenderPrimitiveType::None;
		//const VertexLayout* vertexLayout = nullptr;
		RenderGpuBuffer*	vertexBuffer;
		size_t vertexCount = 0;
	};
}