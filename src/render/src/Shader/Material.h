#pragma once

#include "Shader.h"
#include <RenderGpuBuffer.h>

namespace sge
{
	class RenderContext;
	class Material;
	class MaterialPass;


	struct MaterialPass_Stage : public NonCopyable
	{
		//using M_Pass = MaterialPass;
	public:
		virtual ~MaterialPass_Stage() = default;
		MaterialPass_Stage(MaterialPass* pass, ShaderStage* shaderStage);

		const ShaderDescData* Info() const { return _shaderStage->info(); }

	friend class MaterialPass;
	protected:
		struct ConstBuffer
		{
		public:
			using DataType = RenderDataType;
			using Info = ShaderDescData::ConstBufferInfo;
			using VarInfo = ShaderDescData::ShaderVariable;

			Vector<u8>				cpuBuffer;
			SPtr<RenderGpuBuffer>	gpuBuffer;

			template<class V>
			void setParam(const Info& cbInfo, StrView name, const V& v)
			{
				auto* varInfo = cbInfo.findVariable(name);
				if (!varInfo) return;
				_setParam(varInfo, v);
			}

			void create(const Info& info);
			void uploadToGpu();

			const Info* info() const { return _info; }

		private:
			const Info* _info		= nullptr;
			bool		_gpuDirty	= false;

			void _setParam(const VarInfo* varInfo, const float&   value) { _setParamCheckType(varInfo, value, DataType::Float32); }
			void _setParam(const VarInfo* varInfo, const Tuple2f& value) { _setParamCheckType(varInfo, value, DataType::Float32x2); }
			void _setParam(const VarInfo* varInfo, const Tuple3f& value) { _setParamCheckType(varInfo, value, DataType::Float32x3); }
			void _setParam(const VarInfo* varInfo, const Tuple4f& value) { _setParamCheckType(varInfo, value, DataType::Float32x4); }
			//void _setParam(const VarInfo* varInfo, const Mat4f& value) { _setParamCheckType(varInfo, value, DataType::Float32_4x4); }

			template<class V>
			void _setParamCheckType(const VarInfo* varInfo, const V& value, DataType dataType) {
				if (varInfo->dataType != dataType) {
					errorType();
					return;
				}
				_setValueAs(varInfo, value);
			}

			template<class V>
			void _setValueAs(const VarInfo* varInfo, const V& value) 
			{
				auto end = varInfo->offset + sizeof(value);
				if (end > cpuBuffer.size())
					throw SGE_ERROR("ConstBuffer setParam out of range");
				auto* dst = cpuBuffer.data() + varInfo->offset;
				*reinterpret_cast<V*>(dst) = value;
				_gpuDirty = true;
			}

			void errorType();
		};

		template<class V>
		void _setParam(StrView name, const V& v)
		{
			if (_shaderStage == nullptr) return;
			size_t i = 0;
			for (auto& cb : _constBufs)
			{
				cb.setParam(Info()->constBuffers[i], name, v);
				i++;
			}
		}

		MaterialPass*			_pass = nullptr;
		ShaderStage*			_shaderStage = nullptr;
		Vector_<ConstBuffer, 4> _constBufs;

	};


	struct MaterialPass_VertexStage : public MaterialPass_Stage {
		using Base = MaterialPass_Stage;

		MaterialPass_VertexStage(MaterialPass* pass, ShaderVertexStage* shaderStage)
			: Base(pass, shaderStage)
		{}
	};

	struct MaterialPass_PixelStage : public MaterialPass_Stage {
		using Base = MaterialPass_Stage;
		MaterialPass_PixelStage(MaterialPass* pass, ShaderPixelStage* shaderStage)
			: Base(pass, shaderStage)
		{}
	};

	class MaterialPass : public RefCountBase
	{
	public:
		using Pass = MaterialPass;
		using VertexStage = MaterialPass_VertexStage;
		using PixelStage = MaterialPass_PixelStage;

		virtual ~MaterialPass() = default;

		void bind(RenderContext* ctx, const VertexLayout* vertexLayout) { onBind(ctx, vertexLayout); }

		friend class Material;
	protected:

		MaterialPass(Material* mat, ShaderPass* SahderPass) : _material(mat), _shaderPass(SahderPass) {}

		Material*	_material = nullptr;
		ShaderPass* _shaderPass = nullptr;

		virtual void onBind(RenderContext* ctx, const VertexLayout* vertexLayout) = 0;

		template <class V> 
		void _setParam(StrView name, const V& v)
		{
			if (_vertexStage)	_vertexStage->_setParam(name, v);
			if (_pixelStage)	_pixelStage->_setParam(name, v);
		}

		VertexStage*	_vertexStage	= nullptr;
		PixelStage*		_pixelStage		= nullptr;

	};

	class Material : public RefCountBase
	{
		using Pass			= MaterialPass;
		using Stage			= MaterialPass_Stage;
		using VertexStage	= MaterialPass_VertexStage;
		using PixelStage	= MaterialPass_PixelStage;

	public:
		virtual ~Material() = default;

		void setShader(Shader* shader);
		void setParam(StrView name, const float& v) { _setParam(name, v); }
		void setParam(StrView name, const Tuple4f& v) { _setParam(name, v); }
		void setParam(StrView name, const Color4f& v) { _setParam(name, v.toTuple()); }

		Span<SPtr<Pass>>	passes() { return _passes; }

	protected:
		template<class V> void _setParam(StrView name, const V& v)
		{
			for (auto& pass : _passes)
			{
				if (pass) pass->_setParam(name, v);
			}
		}

		Vector_<SPtr<Pass>, 1> _passes;
		SPtr<Shader> _shader;
		virtual void	onSetShader() {};
		virtual Pass* onCreatePass(Material* mat, ShaderPass* shaderPass) = 0;
	};

}