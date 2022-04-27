#pragma once

#include "sge_core.h"
#
namespace sge {


	struct RenderContext_CreateDesc {
		NativeUIWindow* window = nullptr;
	};


	class RenderContext
	{
	public:
		using CreateDesc = RenderContext_CreateDesc;


		void beginRender() { onBeginRender(); }
		void endRender() { onEndRender(); }
		void render() { onRender(); }

		RenderContext(CreateDesc& desc);
		virtual ~RenderContext() = default;
		

	private:

		virtual void onBeginRender() {};
		virtual void onEndRender() {};
		virtual void onRender() {};

		Vec2f	_frameBufferSize{ 0,0 };
	};


}