#pragma once


#include "RenderCommon_DX11.h"
#include "Renderer.h"


namespace sge {



	class Renderer_DX11 : public Renderer
	{
	public:
		static Renderer_DX11* current() { return static_cast<Renderer_DX11*>(_current); }

		Renderer_DX11(CreateDesc& desc);

		IDXGIFactory1*			dxgiFactory()		{ return _dxgiFactory; }
		IDXGIDevice*			dxgiDevice()		{ return _dxgiDev; }
		IDXGIAdapter3*			dxgiAdapter()		{ return _dxgiAdapter; }

		ID3D11Device1*			d3dDevice()			{ return _d3dDev; }
		ID3D11DeviceContext4*	d3dDeviceContext()	{ return _d3dCtx; }
		//ID3D11Debug*			d3dDebug()			{ return _d3dDebug; }

	protected:
		virtual RenderContext* onCreateContext(RenderContext_CreateDesc& desc) override;
		virtual RenderGpuBuffer* onCreateGpuBuffer(RenderGpuBuffer_CreateDesc& desc) override;

		IDXGIFactory1*			_dxgiFactory;
		IDXGIDevice*			_dxgiDev;
		IDXGIAdapter3*			_dxgiAdapter;
		ID3D11Device1*			_d3dDev;
		ID3D11DeviceContext4*	_d3dCtx;
		//ID3D11Debug*			_d3dDebug;


	};




}
