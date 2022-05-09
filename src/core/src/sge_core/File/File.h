#pragma once

#include "sge_core/string/UtfUtil.h"


namespace sge
{
	enum  class FileMode
	{
		CreateNew,
		OpenExists,
		OpenOrCreate,

	};

	enum class FileAccess
	{
		ReadOnly,
		ReadWrite,
		WriteOnly,
	};

	enum class FileShareMode
	{
		None,
		Read,
		Write,
		ReadWrite,
	};
}