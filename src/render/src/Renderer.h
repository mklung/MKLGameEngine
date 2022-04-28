#pragma once

#include "sge_core.h"
#include "mesh/RenderMesh.h"


namespace sge
{
	class RenderContext;
	struct RenderContext_CreateDesc;


	class Renderer : public NonCopyable
	{
	public:
		static Renderer* current() { return _current; }
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
		RenderContext* createContext(RenderContext_CreateDesc& desc) { return onCreateContext(desc); }
	protected:

		virtual RenderContext* onCreateContext(RenderContext_CreateDesc& desc) = 0;
		static Renderer* _current;

		bool _vsync : 1;
	};


}