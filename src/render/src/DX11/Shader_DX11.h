#pragma once

#include <Shader/Shader.h>
#include <DX11/RenderCommon_DX11.h>
#include "RenderContext_DX11.h"

namespace sge
{
	class Shader_DX11 : public Shader
	{
	public:
		
		Shader_DX11(StrView filename);
		struct  MyPass;

		static void _loadStageFile(StrView passPath, ShaderDescMask mask,
								Vector<u8>& outBytecode, ShaderDescData& outInfo);

		struct MyVertexStage : public ShaderVertexStage
		{
			void load(MyPass* pass, StrView passPath, ID3D11Device1* dev);
			void bind(RenderContext_DX11* ctx);
			ByteSpan bytecode() const { return _bytecode; }
		private:
			ComPtr<ID3D11VertexShader>	_shader;
			Vector<u8> _bytecode;
		};

		struct MyPixelStage : public ShaderPixelStage
		{
			void load(MyPass* pass, StrView passPath, ID3D11Device1* dev);
			void bind(RenderContext_DX11* ctx);
			ByteSpan bytecode() const { return _bytecode; }
		private:
			ComPtr<ID3D11PixelShader>	_shader;
			Vector<u8> _bytecode;
		};

		//struct MyRenderState
		//{
		//	void createState(MyPass* pass, ID3D11Device1* dev);
		//	void bind(RenderContext_DX11* ctx);
		//private:
		//	RenderState*					_renderState;
		//	ComPtr<ID3D11RasterizerState>	_rasterizerState;
		//	ComPtr<ID3D11DepthStencilState> _depthStencilState;
		//	ComPtr<ID3D11BlendState>		_blendState;
		//};

		struct MyPass : public ShaderPass
		{
			MyPass(Shader_DX11* shader, StrView passPath, ShaderInfo::Pass& info, int passIndex);
		private:
			MyVertexStage		_myVertexStage;
			MyPixelStage		_myPixelStage;
			//MyRenderState		_myRenderState;
		};
	};
}