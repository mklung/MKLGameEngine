#include "Shader_DX11.h"
#include "Renderer_DX11.h"

namespace sge
{
	Shader_DX11::Shader_DX11(StrView filename) : Shader(filename)
	{
		auto* proj = ProjectSettings::instance();
		TempString passPath;

		size_t n = _info.passes.size();
		_passes.reserve(n);

		for (size_t i = 0; i < n; i++)
		{
			FmtTo(passPath, "{}/{}", proj->importedPath(), filename);
			auto* pass = new MyPass(this, passPath, _info.passes[i], i);
			_passes.emplace_back(pass);
		}
	}

	void Shader_DX11::_loadStageFile(StrView passPath, ShaderDescMask mask, Vector<u8>& outBytecode, ShaderDescData& outInfo)
	{
		auto* profile = DX11Util::getDxStageProfile(mask);
		auto filename = Fmt("{}/DX11/{}.bin", passPath, profile);
		int a = 0;
		File::readFile(filename, outBytecode);

		filename = Fmt("{}/DX11/{}.json", passPath, profile);
		JsonUtil::readFile(filename, outInfo);
	}

	void Shader_DX11::MyVertexStage::load(MyPass* pass, StrView passPath, ID3D11Device1* dev)
	{
		_loadStageFile(passPath, stageMask(), _bytecode, _info);
		auto hr = dev->CreateVertexShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
		DX11Util::throwIfError(hr);
	}

	void Shader_DX11::MyVertexStage::bind(RenderContext_DX11* ctx)
	{
		auto* dc = ctx->renderer()->d3dDeviceContext();
		if (!_shader) throw SGE_ERROR("dx shader is null");
		dc->VSSetShader(_shader, 0, 0);
	}


	void Shader_DX11::MyPixelStage::load(MyPass* pass, StrView passPath, ID3D11Device1* dev)
	{
		_loadStageFile(passPath, stageMask(), _bytecode, _info);
		auto hr = dev->CreatePixelShader(_bytecode.data(), _bytecode.size(), nullptr, _shader.ptrForInit());
		DX11Util::throwIfError(hr);
	}

	void Shader_DX11::MyPixelStage::bind(RenderContext_DX11* ctx)
	{
		auto* dc = ctx->renderer()->d3dDeviceContext();
		if (!_shader) throw SGE_ERROR("dx shader is null");
		dc->PSSetShader(_shader, 0, 0);
	}


	Shader_DX11::MyPass::MyPass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info, int index) : ShaderPass(shader, info)
	{
		_vertexStage = &_myVertexStage;
		_pixelStage = &_myPixelStage;

		auto* renderer = Renderer_DX11::instance();
		auto* dev = renderer->d3dDevice();

		if (info.vsEntryPt.size()) { _myVertexStage.load(this, passPath, dev); }
		if (info.psEntryPt.size()) { _myPixelStage.load(this, passPath, dev); }
	}


}