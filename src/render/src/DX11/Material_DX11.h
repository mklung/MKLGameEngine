#pragma once

#include <Shader/Material.h>
#include "Shader_DX11.h"
#include "RenderGpuBuffer_DX11.h"


namespace sge
{
	class Material_DX11 : public Material
	{
	public:

	private:
		Shader_DX11* shader() { return static_cast<Shader_DX11*>(_shader.ptr()); }

		template <class STAGE>
		static void _bindStageHelper(RenderContext_DX11* ctx, STAGE* stage);

		struct MyVertexStage : public MaterialPass_VertexStage
		{
			using Base = MaterialPass_VertexStage;
			MyVertexStage(MaterialPass* pass, ShaderVertexStage* shaderstage) : Base(pass, shaderstage) {}

			void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);
			void bindInputLayout(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

			void _dxSetConstBuffer(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11Buffer* d3dBuf)
			{
				dc->VSSetConstantBuffers(bindPoint, 1, &d3dBuf);
			}

			Span<ConstBuffer> constBuffers() { return _constBufs; }
			Shader_DX11::MyVertexStage* shaderStage() { return static_cast<Shader_DX11::MyVertexStage*>(_shaderStage); }

			VectorMap<const VertexLayout*, ComPtr<ID3D11InputLayout>> _inputLayoutsMap;
		};

		struct MyPixelStage : public MaterialPass_PixelStage 
		{
			using Base = MaterialPass_PixelStage;
			using MyShaderStage = Shader_DX11::MyPixelStage;

			MyPixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage) : Base(pass, shaderStage) {}
			void bind(RenderContext_DX11* ctx, const VertexLayout* vertexLayout);

			void _dxSetConstBuffer(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11Buffer* d3dBuf) {
				dc->PSSetConstantBuffers(bindPoint, 1, &d3dBuf);
			}

			Span<ConstBuffer> constBuffers() { return _constBufs; }
			Shader_DX11::MyPixelStage* shaderStage() { return static_cast<Shader_DX11::MyPixelStage*>(_shaderStage); }
		};

		struct MyPass : public MaterialPass 
		{
			MyPass(Material* material, ShaderPass* shaderPass);

			virtual void onBind(RenderContext* _ctx, const VertexLayout* vertexLayout) override;
			//virtual void onBindRenderState(RenderContext* _ctx) override;

			MyVertexStage	_myVertexStage;
			MyPixelStage	_myPixelStage;
			RenderState*	_myRenderState;
		};

		virtual MaterialPass* onCreatePass(Material* material, ShaderPass* shaderPass) override
		{
			return new MyPass(material, shaderPass);
		}

	};

	template<class STAGE>
	inline void Material_DX11::_bindStageHelper(RenderContext_DX11* ctx, STAGE* stage)
	{
		auto* shaderStage = stage->shaderStage();
		if (!shaderStage) return;
		shaderStage->bind(ctx);

		auto* dc = ctx->renderer()->d3dDeviceContext();

		for (auto& cb : stage->constBuffers()) 
		{
			cb.uploadToGpu();

			auto* cbInfo = cb.info();
			UINT bindPoint = cbInfo->bindPoint;

			auto* gpuBuffer = static_cast<RenderGpuBuffer_DX11*>(cb.gpuBuffer.ptr());
			if (!gpuBuffer) throw SGE_ERROR("cosnt buffer is null");

			auto* d3dBuf = gpuBuffer->d3dBuf();
			if (!d3dBuf) throw SGE_ERROR("d3dbuffer is null");

			stage->_dxSetConstBuffer(dc, bindPoint, d3dBuf);
		}
	}

}