#pragma once


#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <Render_Common.h>
#include <Vertex/Vertex.h>
#include <Shader/Shader.h>
#include <texture/Texture.h>


namespace sge {

	class Renderer_DX11;

	struct DX11Util {
		DX11Util() = delete;

		static void throwIfError(HRESULT hr) {
			if (!_checkError(hr)) { reportError(hr); throw SGE_ERROR("HRESULT = {}", hr); }
		}
		static bool assertIfError(HRESULT hr) {
			if (!_checkError(hr)) { reportError(hr); SGE_ASSERT(false); return false; }
			return true;
		}
		static void reportError(HRESULT hr);

		static UINT castUINT(size_t v) { SGE_ASSERT(v < UINT_MAX); return static_cast<UINT>(v); }

		static D3D11_PRIMITIVE_TOPOLOGY		getDxPrimitiveTopology	(RenderPrimitiveType t);
		static DXGI_FORMAT					getDxFormat				(RenderDataType v);

		static DXGI_FORMAT					getDxColorType			(ColorType v);
		static D3D11_FILTER					getDxTextureFilter		(TextureFilter v);
		static D3D11_TEXTURE_ADDRESS_MODE	getDxTextureWrap		(TextureWrap v);


		static const char*					getDxSemanticName		(VertexSemanticType t);
		static VertexSemanticType			parseDxSemanticName		(StrView s);
		static D3D11_CULL_MODE				getDxCull				(ShaderCull c);
		static D3D11_BLEND					getDxBlend				(BlendFactor b);
		static D3D11_COMPARISON_FUNC		getDxDepthTest			(DepthTest d);
		static D3D11_BLEND_OP				getDxBlendOP			(BlendOP b_op);

		static const char* getDxStageProfile(ShaderDescMask s);

		static String getStrFromHRESULT(HRESULT hr);

	private:
		static bool _checkError(HRESULT hr) {
			return SUCCEEDED(hr);
		}
	};

	inline
		String DX11Util::getStrFromHRESULT(HRESULT hr) {
		const int bufSize = 4096;
		wchar_t buf[bufSize + 1];

		DWORD langId = 0; // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, langId, buf, bufSize, nullptr);
		buf[bufSize] = 0; // ensure terminate with 0

		auto str = UtfUtil::toString(buf);
		return str;
	}

	inline
	const char* DX11Util::getDxStageProfile(ShaderDescMask s)
	{
		switch (s) {
		case ShaderDescMask::Vertex:	return "vs_Pass0";
		case ShaderDescMask::Pixel:		return "ps_Pass0";
		default: return "";
		}
	}
	inline
		void DX11Util::reportError(HRESULT hr) {
		if (!SUCCEEDED(hr)) {
			auto str = getStrFromHRESULT(hr);
			SGE_LOG("HRESULT(0x{:0X}) {}", static_cast<u32>(hr), str);
		}

#if 0 && _DEBUG
		auto* d = renderer()->d3dDebug();
		if (d) {
			d->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}
#endif
	}

	inline
		D3D11_PRIMITIVE_TOPOLOGY DX11Util::getDxPrimitiveTopology(RenderPrimitiveType t) {
		using SRC = RenderPrimitiveType;
		switch (t) {
		case SRC::Points:		return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case SRC::Lines:		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case SRC::Triangles:	return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		default: throw SGE_ERROR("unknown RenderPrimitiveType");
		}
	}

	inline
	const char* DX11Util::getDxSemanticName(VertexSemanticType t) 
	{
		using SRC = VertexSemanticType;
		switch (t) {
		case SRC::POSITION:		return "POSITION";
		case SRC::COLOR:		return "COLOR";
		case SRC::TEXCOORD:		return "TEXCOORD";
		case SRC::NORMAL:		return "NORMAL";
		case SRC::TANGENT:		return "TANGENT";
		case SRC::BINORMAL:		return "BINORMAL";
		default: throw SGE_ERROR("unknown VertexLayout_SemanticType");
		}
	}

	inline
	D3D11_CULL_MODE DX11Util::getDxCull(ShaderCull s)
	{
		using SRC = ShaderCull;
		switch (s)
		{
			case SRC::Back:		return D3D11_CULL_BACK;
			case SRC::Front:	return D3D11_CULL_FRONT;
			case SRC::Off:		return D3D11_CULL_NONE;
			default:	throw SGE_ERROR("unknown ShaderCull Type");
		}
	}
	
	inline
	D3D11_BLEND DX11Util::getDxBlend(BlendFactor b)
	{
		using SRC = BlendFactor;
		switch(b)
		{
			case SRC::One:				return D3D11_BLEND_ONE;
			case SRC::Zero:				return D3D11_BLEND_ZERO;
			case SRC::SrcColor:			return D3D11_BLEND_SRC_COLOR;
			case SRC::SrcAlpha:			return D3D11_BLEND_SRC_ALPHA;
			case SRC::OneMinusSrcColor: return D3D11_BLEND_INV_SRC_COLOR;
			case SRC::OneMinusSrcAlpha: return D3D11_BLEND_INV_SRC_ALPHA;

			case SRC::DstColor:			return D3D11_BLEND_DEST_COLOR;
			case SRC::DstAlpha:			return D3D11_BLEND_DEST_ALPHA;
			case SRC::OneMinusDstColor: return D3D11_BLEND_INV_DEST_COLOR;
			case SRC::OneMinusDstAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
			default:	throw SGE_ERROR("unknown BlendFactor Type");
		}
	}

	inline
	D3D11_COMPARISON_FUNC DX11Util::getDxDepthTest(DepthTest d)
	{
		using SRC = DepthTest;
		switch (d)
		{
		case SRC::Less:		return D3D11_COMPARISON_LESS;
		case SRC::Greater:	return D3D11_COMPARISON_GREATER;
		case SRC::LEqual:	return D3D11_COMPARISON_LESS_EQUAL;
		case SRC::GEqual:	return D3D11_COMPARISON_GREATER_EQUAL;
		case SRC::Equal:	return D3D11_COMPARISON_EQUAL;
		case SRC::NotEqual:	return D3D11_COMPARISON_NOT_EQUAL;
		case SRC::Always:	return D3D11_COMPARISON_ALWAYS;
		default:	throw SGE_ERROR("unknown DepthTest Type");
		}
	}

	inline
	D3D11_BLEND_OP DX11Util::getDxBlendOP(BlendOP b_op)
	{
		using SRC = BlendOP;
		switch (b_op)
		{
			case SRC::Add:		return D3D11_BLEND_OP_ADD;
			case SRC::Sub:		return D3D11_BLEND_OP_SUBTRACT;
			case SRC::RevSub:	return D3D11_BLEND_OP_REV_SUBTRACT;
			case SRC::Min:		return D3D11_BLEND_OP_MIN;
			case SRC::Max:		return D3D11_BLEND_OP_MAX;
			default:	throw SGE_ERROR("unknown BlendOP Type");

		}
	}

	inline
		DXGI_FORMAT DX11Util::getDxFormat(RenderDataType v) {
		using SRC = RenderDataType;
		switch (v) {
		case SRC::Int8:			return DXGI_FORMAT_R8_SINT; break;
		case SRC::Int8x2:		return DXGI_FORMAT_R8G8_SINT; break;
//		case SRC::Int8x3:		return DXGI_FORMAT_R8G8B8_SINT; break; //does not support in DX11
		case SRC::Int8x4:		return DXGI_FORMAT_R8G8B8A8_SINT; break;

		case SRC::Int16:		return DXGI_FORMAT_R16_SINT; break;
		case SRC::Int16x2:		return DXGI_FORMAT_R16G16_SINT; break;
//		case SRC::Int16x3:		return DXGI_FORMAT_R16G16B16_SINT; break; //does not support in DX11
		case SRC::Int16x4:		return DXGI_FORMAT_R16G16B16A16_SINT; break;

		case SRC::Int32:		return DXGI_FORMAT_R32_SINT; break;
		case SRC::Int32x2:		return DXGI_FORMAT_R32G32_SINT; break;
//		case SRC::Int32x3:		return DXGI_FORMAT_R32G32B32_SINT; break; //does not support in DX11
		case SRC::Int32x4:		return DXGI_FORMAT_R32G32B32A32_SINT; break;


		case SRC::UInt8:		return DXGI_FORMAT_R8_UINT; break;
		case SRC::UInt8x2:		return DXGI_FORMAT_R8G8_UINT; break;
//		case SRC::UInt8x3:		return DXGI_FORMAT_R8G8B8_UINT; break; //does not support in DX11
		case SRC::UInt8x4:		return DXGI_FORMAT_R8G8B8A8_UINT; break;

		case SRC::UInt16:		return DXGI_FORMAT_R16_UINT; break;
		case SRC::UInt16x2:		return DXGI_FORMAT_R16G16_UINT; break;
//		case SRC::UInt16x3:		return DXGI_FORMAT_R16G16B16_UINT; break; //does not support in DX11
		case SRC::UInt16x4:		return DXGI_FORMAT_R16G16B16A16_UINT; break;

		case SRC::UInt32:		return DXGI_FORMAT_R32_UINT; break;
		case SRC::UInt32x2:		return DXGI_FORMAT_R32G32_UINT; break;
//		case SRC::UInt32x3:		return DXGI_FORMAT_R32G32B32_UINT; break; //does not support in DX11
		case SRC::UInt32x4:		return DXGI_FORMAT_R32G32B32A32_UINT; break;


		case SRC::SNorm8:		return DXGI_FORMAT_R8_SNORM; break;
		case SRC::SNorm8x2:		return DXGI_FORMAT_R8G8_SNORM; break;
//		case SRC::SNorm8x3:		return DXGI_FORMAT_R8G8B8_SNORM; break; //does not support in DX11
		case SRC::SNorm8x4:		return DXGI_FORMAT_R8G8B8A8_SNORM; break;

		case SRC::SNorm16:		return DXGI_FORMAT_R16_SNORM; break;
		case SRC::SNorm16x2:	return DXGI_FORMAT_R16G16_SNORM; break;
//		case SRC::SNorm16x3:	return DXGI_FORMAT_R16G16B16_SNORM; break; //does not support in DX11
		case SRC::SNorm16x4:	return DXGI_FORMAT_R16G16B16A16_SNORM; break;


		case SRC::UNorm8:		return DXGI_FORMAT_R8_UNORM; break;
		case SRC::UNorm8x2:		return DXGI_FORMAT_R8G8_UNORM; break;
//		case SRC::UNorm8x3:		return DXGI_FORMAT_R8G8B8_UNORM; break; //does not support in DX11
		case SRC::UNorm8x4:		return DXGI_FORMAT_R8G8B8A8_UNORM; break;

		case SRC::UNorm16:		return DXGI_FORMAT_R16_UNORM; break;
		case SRC::UNorm16x2:	return DXGI_FORMAT_R16G16_UNORM; break;
//		case SRC::UNorm16x3:	return DXGI_FORMAT_R16G16B16_UNORM; break; //does not support in DX11
		case SRC::UNorm16x4:	return DXGI_FORMAT_R16G16B16A16_UNORM; break;


		case SRC::Float16:		return DXGI_FORMAT_R16_FLOAT; break;
		case SRC::Float16x2:	return DXGI_FORMAT_R16G16_FLOAT; break;
//		case SRC::Float16x3:	return DXGI_FORMAT_R16G16B16_FLOAT; break; //does not support in DX11
		case SRC::Float16x4:	return DXGI_FORMAT_R16G16B16A16_FLOAT; break;

		case SRC::Float32:		return DXGI_FORMAT_R32_FLOAT; break;
		case SRC::Float32x2:	return DXGI_FORMAT_R32G32_FLOAT; break;
		case SRC::Float32x3:	return DXGI_FORMAT_R32G32B32_FLOAT; break;
		case SRC::Float32x4:	return DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			
		default: throw SGE_ERROR("unsupported RenderDataType");
		}
	}

	inline
		DXGI_FORMAT DX11Util::getDxColorType(ColorType v) {
		using SRC = ColorType;
		switch (v) {
		case SRC::Rb:		return DXGI_FORMAT_R8_UNORM;
		case SRC::Rf:		return DXGI_FORMAT_R32_FLOAT;

		case SRC::RGb:		return DXGI_FORMAT_R8G8_UNORM;
		case SRC::RGf:		return DXGI_FORMAT_R32G32_FLOAT;

		//case SRC::RGBb:	return DXGI_FORMAT_R8G8B8_UNORM;		// DX Not Support
		//case SRC::RGBf:	return DXGI_FORMAT_R32G32B32_FLOAT;		// DX Not Support

		case SRC::RGBAb:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case SRC::RGBAf:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			//
		case SRC::BC1:		return DXGI_FORMAT_BC1_UNORM;
		case SRC::BC2:		return DXGI_FORMAT_BC2_UNORM;
		case SRC::BC3:		return DXGI_FORMAT_BC3_UNORM;
		case SRC::BC4:		return DXGI_FORMAT_BC4_UNORM;
		case SRC::BC5:		return DXGI_FORMAT_BC5_UNORM;
		case SRC::BC6h:		return DXGI_FORMAT_BC6H_UF16;
		case SRC::BC7:		return DXGI_FORMAT_BC7_UNORM;

		default: throw SGE_ERROR("unsupported ColorType");
		}
	}

	inline D3D11_FILTER DX11Util::getDxTextureFilter(TextureFilter v) {
		using SRC = TextureFilter;
		switch (v) {
		case SRC::Point:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case SRC::Linear:		return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case SRC::Bilinear:		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case SRC::Trilinear:	return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case SRC::Anisotropic:	return D3D11_FILTER_ANISOTROPIC;
			//---
		default: throw SGE_ERROR("unsupported TextureFilter");
		}
	}

	inline D3D11_TEXTURE_ADDRESS_MODE DX11Util::getDxTextureWrap(TextureWrap v) {
		using SRC = TextureWrap;
		switch (v) {
		case SRC::Repeat:		return D3D11_TEXTURE_ADDRESS_WRAP;
		case SRC::Clamp:		return D3D11_TEXTURE_ADDRESS_CLAMP;
		case SRC::Mirror:		return D3D11_TEXTURE_ADDRESS_MIRROR;
		case SRC::MirrorOnce:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			//---
		default: throw SGE_ERROR("unsupported TextureWrap");
		}
	}

}
