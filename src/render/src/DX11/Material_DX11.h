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

			void _dxSetShaderResource(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11ShaderResourceView* rv) {
				dc->VSSetShaderResources(bindPoint, 1, &rv);
			}

			void _dxSetSampler(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11SamplerState* ss) {
				dc->VSSetSamplers(bindPoint, 1, &ss);
			}


			Span<ConstBuffer> constBuffers() { return _constBufs; }
			Span<TexParam>		texParams() { return _texParams; }


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

			void _dxSetShaderResource(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11ShaderResourceView* rv) {
				dc->PSSetShaderResources(bindPoint, 1, &rv);
			}

			void _dxSetSampler(ID3D11DeviceContext4* dc, UINT bindPoint, ID3D11SamplerState* ss) {
				dc->PSSetSamplers(bindPoint, 1, &ss);
			}

			Span<ConstBuffer> constBuffers() { return _constBufs; }
			Span<TexParam>		texParams() { return _texParams; }

			Shader_DX11::MyPixelStage* shaderStage() { return static_cast<Shader_DX11::MyPixelStage*>(_shaderStage); }
		};


		struct MyPass : public MaterialPass 
		{
			MyPass(Material* material, ShaderPass* shaderPass);

			virtual void onBind(RenderContext* _ctx, const VertexLayout* vertexLayout) override;
			void onBindRenderState(RenderContext_DX11* _ctx);

			MyVertexStage	_myVertexStage;
			MyPixelStage	_myPixelStage;

			ComPtr<ID3D11RasterizerState>		_rasterizerState;
			ComPtr<ID3D11DepthStencilState>		_depthStencilState;
			ComPtr<ID3D11BlendState>			_blendState;
		};

		virtual UPtr<MaterialPass> onCreatePass(ShaderPass* shaderPass) override
		{
			return UPtr<MaterialPass>(new MyPass(this, shaderPass));
		}

	};



}