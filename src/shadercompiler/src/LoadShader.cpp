#include "LoadShader.h"

namespace sge
{
	String LoadShader::onGetExecutableFilename()
	{
		wchar_t tmp[MAX_PATH + 1];
		if (!::GetModuleFileName(nullptr, tmp, MAX_PATH))
			throw SGE_ERROR("");

		String o = UtfUtil::toString(tmp);
		return o;
	}

	String LoadShader::onGetCurrentDir()
	{
		wchar_t tmp[MAX_PATH + 1];
		if (!::GetCurrentDirectory(MAX_PATH, tmp))
			throw SGE_ERROR("getCurrentDir");
		String o = UtfUtil::toString(tmp);
		return o;
	}

	void LoadShader::loadShaderFile(StrView filename)
	{

		MemMapFile mm;
		SGE_LOG("{}", filename);
		mm.open(filename);

		_shaderParse.SetshaderPath(filename);
		_shaderParse.Parse(mm);
		
	}

	void LoadShader::onSetCurrentDir(StrView dir)
	{
		TempStringW tmp = UtfUtil::toStringW(dir);
		::SetCurrentDirectory(tmp.c_str());
	}

}
