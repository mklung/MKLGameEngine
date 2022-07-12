#pragma once
#include "sge_core.h"

namespace sge
{
	struct  ImageInfo
	{
	public:
		Vec2i size{ 0,0 };
		int strideInBytes = 0;
		int mipmapCount = 1;
		//int pixekSizeInBytes() const { return }
		//ColorType
	};
}