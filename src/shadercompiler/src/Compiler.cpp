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

			StrView index = Fmt("{}", i);

			if (shaderData->pass[i].psEntryPt != "")
			{
				String target = "ps_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(), mm.filename().c_str(),
					nullptr, nullptr, 
					shaderData->pass[i].psEntryPt.c_str(), target.data(),
					flage1, flage2, 
					0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				FileStream ps_filestream;

				
				String filePath = "LocalTemp/Shader/DX11/" + shaderData->fileName + "_DX11_Pass" + index.data() + "_vs.bin";
				SGE_LOG("{}", filePath);

				ps_filestream.openWrite(filePath, true);
				auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
				Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
				ps_filestream.writeBytes(p_span);

				ShaderReflect(bytecode, target);
				
			}

			if (shaderData->pass[i].vsEntryPt != "")
			{
				String target = "vs_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(),
					shaderData->fileName.c_str(), nullptr,
					nullptr, shaderData->pass[i].vsEntryPt.c_str(),
					target.data(), flage1, flage2, 0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				FileStream vs_filestream;

				String filePath = "LocalTemp/Shader/DX11/" + shaderData->fileName + "_DX11_Pass" + index.data() + "_ps.bin";
				SGE_LOG("{}", filePath);

				vs_filestream.openWrite(filePath, true);
				auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
				Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());
				vs_filestream.writeBytes(p_span);

				ShaderReflect(bytecode, target);
			}
		}
	}

	void ShaderCompiler::ShaderReflect(ComPtr<ID3DBlob>& byteCode, String profile)
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
			ShaderInputParam sip;

			hr = reflection->GetInputParameterDesc(i, &desc);
			
			sip.dataType = ConvertShaderDataType(&desc);
			sip.attrId = Fmt("{}{}", desc.SemanticName, desc.SemanticIndex);

			StrView type = enumStr(sip.dataType);
			SGE_LOG("attrId:{} ValueType:{}", sip.attrId, type);

			shaderDescData.inputs.emplace_back(sip);
		}

		

		{
			D3D11_SHADER_BUFFER_DESC bufferDesc;
			D3D11_SHADER_VARIABLE_DESC varDesc;
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			for (int i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				ConstBufferDesc constbufDesc;

				auto* constBuf = reflection->GetConstantBufferByIndex(i);
				hr = constBuf->GetDesc(&bufferDesc);

				constbufDesc.name = bufferDesc.Name;
				constbufDesc.dataSize = bufferDesc.Size;
				
				reflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

				constbufDesc.bindPoint = bindDesc.BindPoint;
				constbufDesc.bindCount = bindDesc.BindCount;
				int a = 1;
				for (int j = 0; j < bufferDesc.Variables; j++)
				{
					auto* varBuf = constBuf->GetVariableByIndex(j);
					varBuf->GetDesc(&varDesc);
					ShaderVariable shaderVar;
					shaderVar.name = varDesc.Name;
					shaderVar.offset = varDesc.StartOffset;
					
					continue;
					//varDesc.
				}

				shaderDescData.constBuffers.emplace_back(constbufDesc);
			}
		}
		//SGE_LOG("{}", shaderDescData.inputs[0].attrId);
		//int a = 1;
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