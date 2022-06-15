#include "Renderer.h"

#include <DX11/Renderer_DX11.h>

namespace sge
{

	Renderer* Renderer::_instance = nullptr;


	Renderer::CreateDesc::CreateDesc()
		: multithread(false)
	{
		#if SGE_OS_WINDOWS
				apiType = ApiType::DX11;
		#else
				apiType = ApiType::None;
		#endif
	}
	Renderer* Renderer::create(CreateDesc& desc)
	{
		Renderer* p = nullptr;
		switch (desc.apiType) {
		case ApiType::DX11: p = new Renderer_DX11(desc); break;
		default: throw SGE_ERROR("unsupport graphic api");
		}

		return p;
		return nullptr;
	}

	Renderer::Renderer()
	{
		SGE_ASSERT(_instance == nullptr);
		_instance = this;
		_vsync = true;
	}

	Renderer::~Renderer()
	{
		SGE_ASSERT(_shaders.size() == 0);
		SGE_ASSERT(_instance == this);
		_instance = nullptr;
	}

	//SPtr<Shader> Renderer::createShader(StrView filename) {
	//	TempString tmpName = filename;

	//	auto it = _shaders.find(tmpName.c_str());
	//	if (it != _shaders.end()) {
	//		return it->second;
	//	}

	//	auto s = onCreateShader(tmpName);
	//	_shaders[tmpName.c_str()] = s.ptr();
	//	return s;
	//}

	void Renderer::onShaderDestory(Shader* shader) {
		_shaders.erase(shader->filename().c_str());
	}
}