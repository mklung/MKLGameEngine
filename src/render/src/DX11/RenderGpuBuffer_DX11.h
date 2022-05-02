#pragma once

#include "RenderCommon_DX11.h"
#include "RenderGpuBuffer.h"

namespace sge
{

	class  RenderGpuBuffer_DX11 : public RenderGpuBuffer
	{
	public:

		RenderGpuBuffer_DX11(CreateDesc& desc);

		virtual void onUploadToGpu(Span<const u8> data, size_t offset) override;

		ID3D11Buffer* d3dBuf() { return _d3dBuf; }

	private:
		ID3D11Buffer* _d3dBuf;
	};

}