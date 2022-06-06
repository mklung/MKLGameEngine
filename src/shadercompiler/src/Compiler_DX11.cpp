#pragma once

#include "Compiler_DX11.h"

namespace sge
{
	

	void ShaderCompiler::CompilerShader(ShaderData *shaderData)
	{
		String fileName = shaderData->path.c_str();
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

			String profile = "";
			auto PassIndex = Fmt("{}", i);
			if (shaderData->pass[i].psEntryPt != "")
			{
				profile = "ps_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(), mm.filename().c_str(),
					nullptr, nullptr, 
					shaderData->pass[i].psEntryPt.c_str(), profile.data(),
					flage1, flage2, 
					0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				auto binFileName = Fmt("{}_Pass{}_{}.bin", shaderData->fileName.data(), PassIndex, profile);
				WriteBinFile(bytecode, binFileName.c_str());

				auto jsonFileName = Fmt("{}_Pass{}_{}.json", shaderData->fileName.data(), PassIndex, profile);
				ShaderReflect(bytecode, profile.c_str(), jsonFileName.c_str());
			}

			if (shaderData->pass[i].vsEntryPt != "")
			{
				profile = "vs_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(),
					shaderData->fileName.c_str(), nullptr,
					nullptr, shaderData->pass[i].vsEntryPt.c_str(),
					profile.data(), flage1, flage2, 0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				auto binFileName = Fmt("{}_Pass{}_{}.bin", shaderData->fileName.data(), PassIndex, profile);
				WriteBinFile(bytecode, binFileName.c_str());
				
				auto jsonFileName  = Fmt("{}_Pass{}_{}.json", shaderData->fileName.data(), PassIndex, profile);
				ShaderReflect(bytecode, profile.c_str(), jsonFileName.c_str());
			}
		}
	}


	void ShaderCompiler::ShaderReflect(ComPtr<ID3DBlob>& byteCode, String profile, String fileName)
	{
		ComPtr<ID3D11ShaderReflection> reflection;
		auto hr = D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), IID_PPV_ARGS(reflection.ptrForInit()));
		D3D11_SHADER_DESC shaderDesc;
		hr = reflection->GetDesc(&shaderDesc);

		ShaderDescData shaderDescData;
		//ShaderInputParam> inputParams;

		shaderDescData.profile = profile;
		
		for (int i = 0; i < shaderDesc.InputParameters; i++)
		{
			D11_PARAM_DESC desc;
			auto& sip = shaderDescData.inputs.emplace_back();

			hr = reflection->GetInputParameterDesc(i, &desc);
			
			sip.dataType = ConvertShaderDataType(&desc);
			sip.attrId = Fmt("{}{}", desc.SemanticName, desc.SemanticIndex);

			StrView type = enumStr(sip.dataType);
			SGE_LOG("attrId:{} ValueType:{}", sip.attrId, type);
		}

		

		{
			D3D11_SHADER_BUFFER_DESC bufferDesc;
			D3D11_SHADER_VARIABLE_DESC varDesc;
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			for (int i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				ConstBufferDesc& constbufDesc = shaderDescData.constBuffers.emplace_back();

				auto* constBuf = reflection->GetConstantBufferByIndex(i);
				hr = constBuf->GetDesc(&bufferDesc);

				constbufDesc.name = bufferDesc.Name;
				constbufDesc.dataSize = bufferDesc.Size;
				
				reflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

				constbufDesc.bindPoint = bindDesc.BindPoint;
				constbufDesc.bindCount = bindDesc.BindCount;

				for (int j = 0; j < bufferDesc.Variables - 1; j++)
				{
					auto* varBuf = constBuf->GetVariableByIndex(j);
					varBuf->GetDesc(&varDesc);
					ShaderVariable& shaderVar = constbufDesc.variables.emplace_back();
					shaderVar.name = varDesc.Name;
					shaderVar.offset = varDesc.StartOffset;

					continue;
				}

				int b = 1;
			}
		}

		FileStream filestream;
		auto filePath = Fmt("{}/{}", COMPILE_FILE_PATH, fileName);
		SGE_LOG("{}", shaderDescData.ToJson());
		Span<const u8> p = ByteSpan_make(shaderDescData.ToJson());

		filestream.openWrite(filePath, true);
		
		//Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
	
		filestream.writeBytes(p);
		SGE_LOG("\n");
	}

	void ShaderCompiler::WriteBinFile(ComPtr<ID3DBlob>& bytecode, String fileName)
	{

		FileStream filestream;
		auto filePath = Fmt("{}/{}", COMPILE_FILE_PATH, fileName);
		SGE_LOG("{}", filePath);

		filestream.openWrite(filePath, true);
		auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
		Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
		filestream.writeBytes(p_span);


	}

	RenderDataType ShaderCompiler::ConvertShaderDataType(D11_PARAM_DESC* desc)
	{
		using type = D3D_REGISTER_COMPONENT_TYPE;
		auto mask = desc->Mask;
		int count = 0;
		
		while (mask != 0)
		{
			mask = mask >> 1;
			if(mask != 0) count += 1;
		}

		switch (desc->ComponentType)
		{
		case type::D3D10_REGISTER_COMPONENT_FLOAT32:
			return (RenderDataType)((int)RenderDataType::Float32 + count);
		case type::D3D10_REGISTER_COMPONENT_UINT32:
			return (RenderDataType)((int)RenderDataType::UInt32 + count);
		case type::D3D10_REGISTER_COMPONENT_SINT32:
			return (RenderDataType)((int)RenderDataType::Int32 + count);
		case type::D3D10_REGISTER_COMPONENT_UNKNOWN:
			SGE_LOG_ERROR("unknown Component Type");
			break;
		default:
			SGE_LOG_ERROR("unknown Component Type");
			break;
		}
	}





}