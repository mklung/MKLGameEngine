#pragma once

#include "Compiler.h"
#include "DX11/RenderCommon_DX11.h"
namespace sge
{
	//void ShaderCompiler::onGetEntryPoint(Vector<Token>& t, StrView filename)
	//{

	//	//ShaderFileName = filename;
	//	//for (int i = 0; i < t.size(); i++)
	//	//{
	//	//	if (t[i].value == "VsFunc")
	//	//	{
	//	//		vShaderEntryPt = t[i + 1].value;
	//	//		//SGE_LOG("{}", t[i].value);
	//	//	}
	//	//	if (t[i].value == "PsFunc")
	//	//	{
	//	//		pShaderEntryPt = t[i + 1].value;
	//	//		//SGE_LOG("{}", t[i].value);

	//	//	}
	//	//}

	//	//onCompilerShader();

	//}

	//void ShaderCompiler::onCompilerShader()
	//{
	//	ComPtr<ID3DBlob> ps;
	//	ComPtr<ID3DBlob> vs;

	//	size_t convertedChar = 0;
	//	const char* source = ShaderFileName.data();
	//	size_t charNum = sizeof(char) * ShaderFileName.size() + 1;
	//	wchar_t* shaderFile = new wchar_t[charNum];
	//	mbstate_t state = mbstate_t();
	//	mbsrtowcs(shaderFile, &source, charNum, &state);
	//	
	//	SGE_LOG("{}", shaderFile);

	//	D3DCompileFromFile(shaderFile, 0, 0, vShaderEntryPt.data(), "vs_4_0", 0, 0, vs.ptrForInit(), 0);
	//	D3DCompileFromFile(shaderFile, 0, 0, pShaderEntryPt.data(), "ps_4_0", 0, 0, ps.ptrForInit(), 0);

	//	FileStream ps_filestream;
	//	ps_filestream.openWrite("Library/DX11/Triangle_DX11_ps.bin", true);
	//	auto *p = reinterpret_cast<u8*>(ps->GetBufferPointer());
	//	Span<const u8> p_span = Span<const u8>(p , ps->GetBufferSize());
	//	ps_filestream.writeBytes(p_span);
	//	ps_filestream.close();


	//	FileStream vs_filestream;
	//	vs_filestream.openWrite("Library/DX11/Triangle_DX11_vs.bin", true);
	//	auto* v = reinterpret_cast<u8*>(vs->GetBufferPointer());
	//	Span<const u8> v_span = Span<const u8>(v, vs->GetBufferSize());
	//	vs_filestream.writeBytes(v_span);
	//	vs_filestream.close();
	//}

}