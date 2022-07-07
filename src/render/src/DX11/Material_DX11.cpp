#include "Material_DX11.h"
#include "Renderer_DX11.h"
#include "RenderContext_DX11.h"

namespace sge
{
	void Material_DX11::MyVertexStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout)
	{
		_bindStageHelper(ctx, this);
		bindInputLayout(ctx, vertexLayout);
	}

	void Material_DX11::MyPixelStage::bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout)
	{
		_bindStageHelper(ctx, this);
	}

	void Material_DX11::MyVertexStage::bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout)
	{
		if (!vertexLayout) throw SGE_ERROR("VertexLayer is null");

		auto* dev = ctx->renderer()->d3dDevice();
		auto* dc = ctx->renderer()->d3dDeviceContext();

		ID3D11InputLayout* dxLayout = nullptr;

		auto it = _inputLayoutsMap.find(vertexLayout);

		if (it != _inputLayoutsMap.end())
		{
			dxLayout = it->second;
		}
		else
		{
			Vector_<D3D11_INPUT_ELEMENT_DESC, 32> inputDesc;

			for (auto& e : vertexLayout->elements)
			{
				auto& dst = inputDesc.emplace_back();
				auto semanticType = Vertex_SemanticUtil::getType(e.semantic);
				dst.SemanticName = DX11Util::getDxSemanticName(semanticType);
				dst.SemanticIndex = Vertex_SemanticUtil::getIndex(e.semantic);
				dst.Format = DX11Util::getDxFormat(e.dataType);
				dst.InputSlot = 0;
				dst.AlignedByteOffset = e.offset;
				dst.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				dst.InstanceDataStepRate = 0;
			}

			ComPtr<ID3D11InputLayout> outLayout;

			auto* ss = static_cast<Shader_DX11::MyVertexStage*>(_shaderStage);
			auto bytecode = ss->bytecode();

			auto hr = dev->CreateInputLayout(inputDesc.data()
				, static_cast<UINT>(inputDesc.size())
				, bytecode.data()
				, bytecode.size()
				, outLayout.ptrForInit());

			DX11Util::throwIfError(hr);
			
			dxLayout = outLayout;
			_inputLayoutsMap[vertexLayout] = std::move(outLayout);

		}

		dc->IASetInputLayout(dxLayout);

	}



	Material_DX11::MyPass::MyPass(Material* material, ShaderPass* shaderPass) : MaterialPass(material, shaderPass)
		, _myVertexStage(this, shaderPass->vertexStage())
		, _myPixelStage(this, shaderPass->pixelStage())
	{
		_vertexStage	= &_myVertexStage;
		_pixelStage		= &_myPixelStage;
		

	}

	void Material_DX11::MyPass::onBind(RenderContext* _ctx, const VertexLayout* vertexLayout)
	{
		auto* ctx = static_cast<RenderContext_DX11*>(_ctx);
		_myVertexStage.bind(ctx, vertexLayout);
		_myPixelStage.bind(ctx, vertexLayout);
		

	}

	void Material_DX11::MyPass::onBindRenderState(RenderContext* ctx)
	{
		{


		}
	}



}