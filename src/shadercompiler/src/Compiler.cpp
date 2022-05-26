#pragma once

#include "Compiler.h"

namespace sge
{
	void ShaderCompiler::CompilerShader(ShaderData *shaderData)
	{
		String fileName = shaderData->path;
		MemMapFile mm;
		mm.open(fileName);
		auto hlsl = StrView(reinterpret_cast<const char*>(mm.data()), mm.size());

		ComPtr<ID3DBlob> bytecode;
		ComPtr<ID3DBlob> errorMsg;

		int passSize = shaderData->pass.size();

		UINT flage1 = 0;
		UINT flage2 = 0;
		SGE_LOG("{}", shaderData->pass[0].psEntryPt.c_str());

		for (int i = 0; i < passSize; i++)
		{
			if (shaderData->pass[i].psEntryPt != "")
			{
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(), mm.filename().c_str(),
					nullptr, nullptr, 
					shaderData->pass[i].psEntryPt.c_str(), "ps_4_0", 
					flage1, flage2, 
					0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				FileStream ps_filestream;
				
				String filePath = "Library/Shader/DX11/" + shaderData->fileName + "_Pass";
				filePath.append("_DX11_vs.bin");
				SGE_LOG("{}", filePath);

				ps_filestream.openWrite(filePath, true);
				auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
				Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
				ps_filestream.writeBytes(p_span);

				ShaderReflect(bytecode);
				
			}

			if (shaderData->pass[i].vsEntryPt != "")
			{
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(),
					shaderData->fileName.c_str(), nullptr,
					nullptr, shaderData->pass[i].vsEntryPt.c_str(),
					"vs_4_0", flage1, flage2, 0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				FileStream vs_filestream;
				String filePath = "Library/Shader/DX11/" + shaderData->fileName + "_Pass" + "_DX11_vs.bin";
				vs_filestream.openWrite(filePath, true);
				auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
				Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
				vs_filestream.writeBytes(p_span);

				ShaderReflect(bytecode);
			}
		}
	}

	void ShaderCompiler::ShaderReflect(ComPtr<ID3DBlob>& byteCode)
	{
		ComPtr<ID3D11ShaderReflection> reflection;
		auto hr = D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), IID_PPV_ARGS(reflection.ptrForInit()));
		D3D11_SHADER_DESC shaderDesc;
		hr = reflection->GetDesc(&shaderDesc);

	}





}