#include "LoadShader.h"

namespace sge

{
	//void LoadShader::loadShaderFile(StrView fileName)
	//{
	//	MemMapFile mm;
	//	mm.open(fileName);

	//	SGE_LOG("{}", mm.data());
	//}

	//void LoadShader::setCurrentDir()
	//{
	//	String file = getExecutableFilename();
	//	String _path = FilePath::getDir(file);
	//	_path.append("/../../../../../../Assets");
	//	onSetCurrentDir(_path);
	//}




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
		mm.open(filename);
		SGE_LOG("{}", mm.data());
	}

	void LoadShader::onSetCurrentDir(StrView dir)
	{
		TempStringW tmp = UtfUtil::toStringW(dir);
		::SetCurrentDirectory(tmp.c_str());
	}

}
