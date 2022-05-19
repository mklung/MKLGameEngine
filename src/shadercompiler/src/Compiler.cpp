#pragma once

#include "Compiler.h"
#include "DX11/RenderCommon_DX11.h"
namespace sge
{
	void ShaderCompiler::onGetEntryPoint(Vector<Token>& t)
	{



		for (int i = 0; i < t.size(); i++)
		{
			if (t[i].value == "VsFunc")
			{
				vShaderEntryPt = t[i + 1].value;
				SGE_LOG("{}", t[i].value);
			}
			if (t[i].value == "PsFunc")
			{
				pShaderEntryPt = t[i + 1].value;
				SGE_LOG("{}", t[i].value);

			}
		}

		onCompilerShader();

	}

	void ShaderCompiler::onCompilerShader()
	{
		ComPtr<ID3DBlob> ps;
		ComPtr<ID3DBlob> vs;

		D3DCompileFromFile(L"Shader/Triangle.hlsl", 0, 0, "vs_main", "vs_4_0", 0, 0, vs.ptrForInit(), 0);
		D3DCompileFromFile(L"Shader/Triangle.hlsl", 0, 0, "ps_main", "ps_4_0", 0, 0, ps.ptrForInit(), 0);

		FileStream ps_filestream;
		ps_filestream.openWrite("Library/DX11/Triangle_DX11_ps.bin", true);
		auto *p = reinterpret_cast<u8*>(ps->GetBufferPointer());
		Span<const u8> p_span = Span<const u8>(p , ps->GetBufferSize());
		ps_filestream.writeBytes(p_span);
		ps_filestream.close();


		FileStream vs_filestream;
		vs_filestream.openWrite("Library/DX11/Triangle_DX11_vs.bin", true);
		auto* v = reinterpret_cast<u8*>(vs->GetBufferPointer());
		Span<const u8> v_span = Span<const u8>(v, vs->GetBufferSize());
		vs_filestream.writeBytes(v_span);
		vs_filestream.close();
	}

}