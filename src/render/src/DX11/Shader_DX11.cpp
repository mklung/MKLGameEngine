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

		//createState();
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


	void Shader_DX11::MyRenderState::createState(MyPass* pass, ID3D11Device1* dev)
	{
		_renderState = &pass->passInfo()->renderState;
		
		if (!_rasterizerState)
		{
			ShaderCull cull;
			enumTryParse(cull, _renderState->cull);

			D3D11_RASTERIZER_DESC rasterDesc = {};
			rasterDesc.AntialiasedLineEnable = true;
			rasterDesc.CullMode = DX11Util::getDxCull(cull);
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;

			bool wireframe = true;
			rasterDesc.FillMode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;

			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.ScissorEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			dev->CreateRasterizerState(&rasterDesc, _rasterizerState.ptrForInit());
		}

		if (!_depthStencilState)
		{
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

			DepthTest depthTest;
			enumTryParse(depthTest, _renderState->depthTest);
			
			if (_renderState->depthWrite == "On")
			{
				depthStencilDesc.DepthEnable = true;
				depthStencilDesc.DepthFunc = DX11Util::getDxDepthTest(depthTest);
			}
			else
			{
				depthStencilDesc.DepthEnable = false;
			}

			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.StencilEnable = false;
			depthStencilDesc.StencilReadMask = 0xFF;
			depthStencilDesc.StencilWriteMask = 0xFF;

			dev->CreateDepthStencilState(&depthStencilDesc, _depthStencilState.ptrForInit());

		}

		if (_blendState)
		{
			RenderState::Blend* colorBlend;
			RenderState::Blend* alphaBlend;

			colorBlend = &pass->passInfo()->renderState.blendRGB;
			alphaBlend = &pass->passInfo()->renderState.blendAlpha;

			D3D11_BLEND_DESC blendStateDesc = {};
			blendStateDesc.AlphaToCoverageEnable = false;
			blendStateDesc.IndependentBlendEnable = false;
			auto& rtDesc = blendStateDesc.RenderTarget[0];
			rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			BlendOP blendOP;
			BlendFactor srcblend;
			BlendFactor destblend;

			enumTryParse(blendOP,	colorBlend->op);
			enumTryParse(srcblend,	colorBlend->src);
			enumTryParse(destblend, colorBlend->dest);

			rtDesc.BlendEnable = true;
			rtDesc.BlendOp		= DX11Util::getDxBlendOP(blendOP);
			rtDesc.SrcBlend		= DX11Util::getDxBlend(srcblend);
			rtDesc.DestBlend	= DX11Util::getDxBlend(destblend);

			enumTryParse(blendOP,	alphaBlend->op);
			enumTryParse(srcblend,	alphaBlend->src);
			enumTryParse(destblend, alphaBlend->dest);

			rtDesc.BlendOpAlpha		= DX11Util::getDxBlendOP(blendOP);
			rtDesc.SrcBlendAlpha	= DX11Util::getDxBlend(srcblend);
			rtDesc.DestBlendAlpha	= DX11Util::getDxBlend(destblend);

			dev->CreateBlendState(&blendStateDesc, _blendState.ptrForInit());
		}
	}

	void Shader_DX11::MyRenderState::bind(RenderContext_DX11* ctx)
	{
		auto* dc = ctx->renderer()->d3dDeviceContext();

		Color4f blendColor(1, 1, 1, 1);

		if(!_rasterizerState)	dc->RSSetState(_rasterizerState);
		if(!_depthStencilState) dc->OMSetDepthStencilState(_depthStencilState, 1);
		if(!_blendState)		dc->OMSetBlendState(_blendState, blendColor.data, 0xffffffff);
	}


}