#pragma once

#include "sge_core.h"
#include "mesh/RenderMesh.h"
#include <Shader/Material.h>

namespace sge
{
	class RenderContext;
	struct RenderContext_CreateDesc;

	class RenderGpuBuffer;
	struct RenderGpuBuffer_CreateDesc;

	class Renderer : public NonCopyable
	{
	public:
		static Renderer* instance() { return s_instance; }
		Renderer();
		~Renderer();

		enum class ApiType {
			None,
			DX11,
			OpenGL,
		};

		struct CreateDesc {
			CreateDesc();
			ApiType		apiType;
			bool multithread : 1;
		};
		
		static Renderer* create(CreateDesc& desc);
		bool vsync() const { return _vsync; }

		Vector_<u8, 1024> vertexData;
		RenderContext*		createContext(RenderContext_CreateDesc& desc)		{ return onCreateContext(desc); }
		RenderGpuBuffer*	createGpuBuffer(RenderGpuBuffer_CreateDesc& desc)	{ return onCreateGpuBuffer(desc); }
		SPtr<Shader>		createShader(StrView filename);
		SPtr<Material>		createMaterial() { return onCreateMaterial(); }

		void onShaderDestory(Shader* shader);

	protected:

		virtual RenderContext*		onCreateContext(RenderContext_CreateDesc& desc) = 0;
		virtual RenderGpuBuffer*	onCreateGpuBuffer(RenderGpuBuffer_CreateDesc& desc) = 0;
		virtual SPtr<Shader>		onCreateShader(StrView filename) = 0;
		virtual SPtr<Material>		onCreateMaterial() = 0;

		StringMap<Shader*>	_shaders;
		static Renderer* s_instance;

		bool _vsync : 1;
	};


}