//#include "Material.h"
//#include <Renderer.h>
//
//namespace sge
//{
//	void Material::setShader(Shader* shader)
//	{
//		if (_shader == shader) return;
//		_shader = shader;
//		_passes.clear();
//		_passes.reserve(shader->passes().size());
//		for (auto& shaderpass : shader->passes())
//		{
//			auto* pass = onCreatePass(this, shaderpass.get());
//			_passes.emplace_back(pass);
//
//		}
//
//		onSetShader();
//	}
//
//	MaterialPass_Stage::MaterialPass_Stage(MaterialPass* pass, ShaderStage* shaderStage) : _pass(pass), _shaderStage(shaderStage)
//	{
//		auto* info = shaderStage->info();
//		auto cbCount = info->constBuffers.size();
//
//		_constBufs.resize(cbCount);
//		
//		for (size_t i = 0; i < cbCount; i++)
//		{
//			auto& cb = _constBufs[i];
//			cb.create(info->constBuffers[i]);
//		}
//	}
//
//
//	void MaterialPass_Stage::ConstBuffer::create(const Info& info)
//	{
//		_info = &info;
//		_gpuDirty = true;
//
//		cpuBuffer.resize(info.dataSize);
//
//		RenderGpuBuffer::CreateDesc desc;
//		desc.type = RenderGpuBufferType::Const;
//		desc.bufferSize = info.dataSize;
//
//		gpuBuffer = Renderer::current()->createGpuBuffer(desc);
//	}
//
//	void MaterialPass_Stage::ConstBuffer::uploadToGpu()
//	{
//		if (!_gpuDirty) return;
//		_gpuDirty = false;
//
//		if (!gpuBuffer) return;
//		gpuBuffer->uploadToGpu(cpuBuffer);
//
//	}
//
//	void MaterialPass_Stage::ConstBuffer::_setParam(const VarInfo* varInfo, const float& value)
//	{
//		switch (varInfo->dataType) {
//		case DataType::Float32: _setValueAs(varInfo, value); break;
//		default: errorType();
//		}
//	}
//
//	void MaterialPass_Stage::ConstBuffer::_setParam(const VarInfo* varInfo, const Tuple4f& value)
//	{
//		switch (varInfo->dataType) {
//		case DataType::Float32x4: _setValueAs(varInfo, value); break;
//		default: errorType();
//		}
//	}
//
//	void MaterialPass_Stage::ConstBuffer::errorType()
//	{
//		throw SGE_ERROR("ConstBuffer setParam type mismatch");
//	}
//
//
//}