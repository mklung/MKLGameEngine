#pragma once

#include "Compiler_DX11.h"

namespace sge
{
	

	void ShaderCompiler::CompilerShader(ShaderInfo *shaderData)
	{
		String fileName = shaderData->path.c_str();
		MemMapFile mm;
		mm.open(fileName);
		auto hlsl = StrView(reinterpret_cast<const char*>(mm.data()), mm.size());

		ComPtr<ID3DBlob> bytecode;
		ComPtr<ID3DBlob> errorMsg;

		int passSize = shaderData->passes.size();

		UINT flage1 = 0;
		UINT flage2 = 0;
		SGE_LOG("{}", shaderData->passes[0].psEntryPt.c_str());

		for (int i = 0; i < passSize; i++)
		{

			String profile = "";
			auto PassIndex = Fmt("{}", i);
			if (shaderData->passes[i].psEntryPt != "")
			{
				profile = "ps_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(), mm.filename().c_str(),
					nullptr, nullptr, 
					shaderData->passes[i].psEntryPt.c_str(), profile.data(),
					flage1, flage2, 
					0, nullptr, 0,
					bytecode.ptrForInit(), errorMsg.ptrForInit());

				auto binFileName = Fmt("{}_Pass{}_{}.bin", shaderData->fileName.data(), PassIndex, profile);
				WriteBinFile(bytecode, binFileName.c_str());

				auto jsonFileName = Fmt("{}_Pass{}_{}.json", shaderData->fileName.data(), PassIndex, profile);
				ShaderReflect(bytecode, profile.c_str(), jsonFileName.c_str());
			}

			if (shaderData->passes[i].vsEntryPt != "")
			{
				profile = "vs_5_0";
				auto hr = D3DCompile2(
					hlsl.data(), hlsl.size(),
					shaderData->fileName.c_str(), nullptr,
					nullptr, shaderData->passes[i].vsEntryPt.c_str(),
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
		
		shaderDescData.inputs.reserve(shaderDesc.InputParameters);

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
			shaderDescData.constBuffers.reserve(shaderDesc.ConstantBuffers);

			for (int i = 0; i < shaderDesc.ConstantBuffers; i++)
			{
				D3D11_SHADER_BUFFER_DESC bufferDesc;
				D3D11_SHADER_INPUT_BIND_DESC bindDesc;

				ConstBufferDesc& constbufDesc = shaderDescData.constBuffers.emplace_back();

				auto* constBuf = reflection->GetConstantBufferByIndex(i);

				hr = constBuf->GetDesc(&bufferDesc);
				DX11Util::throwIfError(hr);

				hr = reflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);
				DX11Util::throwIfError(hr);

				constbufDesc.name		= bufferDesc.Name;
				constbufDesc.dataSize	= bufferDesc.Size;
				constbufDesc.bindPoint	= bindDesc.BindPoint;
				constbufDesc.bindCount	= bindDesc.BindCount;


				constbufDesc.variables.reserve(bufferDesc.Variables);
				for (int j = 0; j < bufferDesc.Variables; j++)
				{
					auto* varBuf = constBuf->GetVariableByIndex(j);
					D3D11_SHADER_VARIABLE_DESC varDesc;
					hr = varBuf->GetDesc(&varDesc);
					DX11Util::throwIfError(hr);

					D3D11_SHADER_TYPE_DESC varType;
					hr = varBuf->GetType()->GetDesc(&varType);
					DX11Util::throwIfError(hr);

					if (0 == (varDesc.uFlags & D3D_SVF_USED)) continue;

					ShaderVariable& shaderVar = constbufDesc.variables.emplace_back();
					shaderVar.name = varDesc.Name;
					shaderVar.offset = varDesc.StartOffset;

					TempString dataType;
					switch (varType.Type)
					{
						case D3D_SVT_BOOL:		dataType.append("Bool");		break;
						case D3D_SVT_INT:		dataType.append("Int32");		break;
						case D3D_SVT_UINT:		dataType.append("UInt32");		break;
						case D3D_SVT_UINT8:		dataType.append("UInt8");		break;
						case D3D_SVT_FLOAT:		dataType.append("Float32");		break;
						case D3D_SVT_DOUBLE:	dataType.append("Float64");		break;
						default: throw SGE_ERROR("unsupported type {}", varType.Type);
					}

					switch (varType.Class) 
					{
						case D3D_SVC_SCALAR: break;
						case D3D_SVC_VECTOR:			FmtTo(dataType, "x{}",		varType.Columns); break;
						case D3D_SVC_MATRIX_COLUMNS:	FmtTo(dataType, "_{}x{}",	varType.Rows, varType.Columns); break;
						case D3D_SVC_MATRIX_ROWS:		FmtTo(dataType, "_{}x{}",	varType.Rows, varType.Columns); break;
						default: throw SGE_ERROR("unsupported Class {}", varType.Class);
					}

					if (!enumTryParse(shaderVar.dataType, dataType)) {
						throw SGE_ERROR("cannot parse dataType {}", dataType);
					}

					if (shaderVar.dataType == RenderDataType::None) {
						throw SGE_ERROR("dataType is None");
					}

					continue;
				}

			}
		}

		SGE_LOG("{}", shaderDescData.ToJson());

		auto filePath = Fmt("{}/{}", COMPILE_FILE_PATH, fileName);
		File::writeFile(filePath, shaderDescData.ToJson(), false);

		SGE_LOG("\n");
	}

	void ShaderCompiler::WriteBinFile(ComPtr<ID3DBlob>& bytecode, String fileName)
	{
		auto filePath = Fmt("{}/{}", COMPILE_FILE_PATH, fileName);
		SGE_LOG("{}", filePath);
		auto* p = reinterpret_cast<u8*>(bytecode->GetBufferPointer());
		Span<const u8> p_span = Span<const u8>(p, bytecode->GetBufferSize());

		File::writeFile(filePath, p_span, false);
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