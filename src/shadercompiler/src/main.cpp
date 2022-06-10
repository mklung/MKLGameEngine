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
	path.append("/../../../../../Data");

	sge::Directory::setCurrent(path);
	//loadShader.setCurrentDir(path);

	auto dir = loadShader.getCurrentDir();

	SGE_LOG("dir : {} ", dir);
	loadShader.loadShaderFile("Assets/Shader/Triangle.hlsl");


	int a;
	scanf("%d", &a);
	return;
}

