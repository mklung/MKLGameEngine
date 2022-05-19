#pragma once
#include "sge_core.h"
#include "LoadShader.h"

namespace sge
{

}
void main()
{
	sge::LoadShader loadShader;
	sge::String file = loadShader.getExecutableFilename();
	sge::String path = sge::FilePath::getDir(file);
	path.append("/../../../../../Assets");
	loadShader.setCurrentDir(path);

	auto dir = loadShader.getCurrentDir();

	loadShader.loadShaderFile("Shader/Triangle.hlsl");
	SGE_LOG("dir : {} ", dir);


	int a;
	scanf("%d", &a);
	return;
}

