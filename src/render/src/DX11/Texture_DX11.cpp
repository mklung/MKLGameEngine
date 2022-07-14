#include "Texture_DX11.h"

namespace sge
{
	Texture2D_DX11::Texture2D_DX11(CreateDesc& desc) : Base(desc)
	{
		auto* renderer = Renderer_DX11::instance();
		auto* dev = renderer->d3dDevice();

		D3D11_TEXTURE2D_DESC dxDesc = {};
		dxDesc.Width		= desc.size.x;
		dxDesc.Width		= desc.size.y;
		dxDesc.MipLevels	= desc.mipmapCount;

		dxDesc.ArraySize = 1;
		dxDesc.Format = DX11Util::getDxColorType(desc.colorType);

		dxDesc.SampleDesc.Count = 1;
		dxDesc.SampleDesc.Quality = 0;

	}

}