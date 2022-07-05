#pragma once

#include "ShaderData.h"

namespace sge
{
	class Shader;
	struct ShaderStage : public NonCopyable
	{
		const ShaderDescData* info() const { return &_info; }
	protected:
		ShaderDescData _info;
	};

	struct ShaderVertexStage : public ShaderStage {
		static constexpr ShaderDescMask stageMask() { return ShaderDescMask::Vertex; }
	};
	struct ShaderPixelStage : public ShaderStage {
		static constexpr ShaderDescMask stageMask() { return ShaderDescMask::Pixel; }
	};


	struct ShaderPass : public NonCopyable {
		ShaderPass(Shader* shader, ShaderInfo::Pass& info);

		virtual ~ShaderPass() = default;

		ShaderVertexStage*	vertexStage()	{ return _vertexStage; }
		ShaderPixelStage*	pixelStage()	{ return _pixelStage; }
		ShaderInfo::Pass*	passInfo()		{ return _info; }
	protected:
		Shader*				_shader			= nullptr;
		ShaderInfo::Pass*	_info			= nullptr;
		ShaderVertexStage*	_vertexStage	= nullptr;
		ShaderPixelStage*	_pixelStage		= nullptr;
	};


	class Shader : public RefCountBase 
	{
	public:
		Shader(StrView filename);
		virtual ~Shader();

		const String& filename() const { return _filename; }

		const ShaderInfo* info() const { return &_info; }

		Span<UPtr<ShaderPass>>	passes() { return _passes; }

	protected:
		String	_filename;
		ShaderInfo	_info;
		Vector_<UPtr<ShaderPass>, 1> _passes;
		virtual void bindRenderState() {};
	};
}