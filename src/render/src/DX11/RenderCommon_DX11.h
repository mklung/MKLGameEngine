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

		static D3D11_PRIMITIVE_TOPOLOGY	getDxPrimitiveTopology(RenderPrimitiveType t);
		static DXGI_FORMAT				getDxFormat(RenderDataType v);

		
		static const char*				getDxSemanticName(VertexSemanticType t);
		static VertexSemanticType		parseDxSemanticName(StrView s);

		static String					getStrFromHRESULT(HRESULT hr);
		static const char*				getDxStageProfile(ShaderDescMask s);

		static ByteSpan toSpan(ID3DBlob* blob);
		static StrView  toStrView(ID3DBlob* blob) { return StrView_make(toSpan(blob)); }

	private:
		static bool _checkError(HRESULT hr) {
			return SUCCEEDED(hr);
		}
	};

	inline
		ByteSpan DX11Util::toSpan(ID3DBlob* blob) {
		if (!blob) return ByteSpan();
		return ByteSpan(reinterpret_cast<const u8*>(blob->GetBufferPointer()),
			static_cast<size_t>(blob->GetBufferSize()));
	}

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
		const char* DX11Util::getDxStageProfile(ShaderDescMask s) {
		switch (s) {
		case ShaderDescMask::Vertex:	return "vs_5_0";
		case ShaderDescMask::Pixel:		return "ps_5_0";
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

	//inline
	//	const char* DX11Util::getDxSemanticName(VertexSemanticType t) {
	//	using SRC = VertexSemanticType;
	//	switch (t) {
	//	case SRC::POSITION:			return "POSITION";
	//	case SRC::COLOR:		return "COLOR";
	//	case SRC::TEXCOORD:		return "TEXCOORD";
	//	case SRC::NORMAL:		return "NORMAL";
	//	case SRC::TANGENT:		return "TANGENT";
	//	case SRC::BINORMAL:		return "BINORMAL";
	//	default: throw SGE_ERROR("unknown VertexLayout_SemanticType");
	//	}
	//}

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
}
