#include "Shader.h"
#include "Renderer.h"

namespace sge
{
	Shader::Shader(StrView filename)
		: _filename(filename)
	{
		auto* proj = ProjectSettings::instance();
		auto infoFilename = Fmt("{}/{}/info.json", proj->importedPath(), filename);
		JsonUtil::readFile(infoFilename, _info);
	}

	Shader::~Shader() {
		auto* renderer = Renderer::current();
		//renderer->onShaderDestory(this);
	}

	ShaderPass::ShaderPass(Shader* shader, ShaderInfo::Pass& info)
		: _shader(shader)
		, _info(&info)
	{

	}
}