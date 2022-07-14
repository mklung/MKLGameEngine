#pragma once

#include "texture/Texture.h"
#include "RenderCommon_DX11.h"
#include "Renderer_DX11.h"

namespace sge
{
	class Texture2D_DX11 : public Texture2D
	{
		using Base = Texture2D;
		using Util = DX11Util;
	public:
		Texture2D_DX11(CreateDesc& desc);

		ID3D11ShaderResourceView* resourceView() { return _resourceView.ptr(); }
		ID3D11SamplerState* samplerState() { return _samplerState.ptr(); }

	private:
		ComPtr<ID3D11Texture2D>				_tex;
		ComPtr<ID3D11ShaderResourceView>	_resourceView;
		ComPtr<ID3D11SamplerState>			_samplerState;
	};


}