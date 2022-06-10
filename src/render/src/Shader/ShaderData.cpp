#pragma once

#include "ShaderData.h"

namespace sge
{


	void ShaderDescData::Clear()
	{
		profile = "";
		inputs.clear();
		constBuffers.clear();
	}

	void ShaderInfo::clear()
	{
		props.clear();
		passes.clear();

	}


}