#pragma once

#include "ShaderData.h"

namespace sge
{
	Json ConstBufferDesc::ToJson()
	{
		Json j;
		j["name"]		= name.c_str();
		j["bindPoint"]	= bindPoint;
		j["bindCount"]	= bindCount;
		j["dataSize"]	= dataSize;
		for (auto& ele : variables)
		{
			j["variables"].emplace_back(ele.ToJson());
		}
		return j;
	}

	Json ShaderInputParam::ToJson()
	{
		Json j;
		j["name"] = name.c_str();
		j["attrId"] = attrId.c_str();
		j["dataType"] = enumStr(dataType).data();
		return j;
	}


	String ShaderDescData::ToJson()
	{
		Json j;
		j["profile"] = profile.c_str();
		for (auto& ele : inputs)
		{
			j["inputs"].emplace_back(ele.ToJson());
		}

		for (auto& ele : constBuffers)
		{
			j["constBuf"].emplace_back(ele.ToJson());
		}

		auto str = j.dump();
		return StrView(str.data(), str.size()).data();
	}

	void ShaderDescData::Clear()
	{
		profile = "";
		inputs.clear();
		constBuffers.clear();
	}


	Json ShaderVariable::ToJson()
	{
		Json j;
		j["name"] = name.c_str();
		j["offset"] = offset;
		j["dataType"] = enumStr(dataType).data();
		return j;
	}


}