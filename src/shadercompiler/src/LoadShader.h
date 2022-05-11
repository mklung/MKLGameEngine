#pragma once

#include "sge_core.h"

namespace sge
{
	class LoadShader;

	class LoadShader
	{
	public:
		void	loadShaderFile(StrView filename);

		void	setCurrentDir(StrView dir)	{ onSetCurrentDir(dir); }

		String	getCurrentDir()				{ return onGetCurrentDir(); }
		String	getExecutableFilename()		{ return onGetExecutableFilename(); }

	private:
		void	onSetCurrentDir(StrView dir);
		
		String	onGetExecutableFilename();
		String	onGetCurrentDir();
	};
}