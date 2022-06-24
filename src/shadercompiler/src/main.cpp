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
	SGE_LOG("dir : {} ", path);
	path.append("/../../../../../Data");

	auto* proj =  sge::ProjectSettings::instance();
	proj->setProjectRoot(path);

	auto dir = loadShader.getCurrentDir();

	SGE_LOG("dir : {} ", dir);
	loadShader.loadShaderFile("Assets/Shader/Standard.shader");


	int a;
	scanf("%d", &a);
	return;
}

