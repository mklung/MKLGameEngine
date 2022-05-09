#pragma once

#include "FileStream.h"

namespace sge
{
	class MemMapFile : NonCopyable
	{
	public:
		~MemMapFile() { close(); }
		void open(StrView filename);
		void close();

		const u8* data()	const { return _span.data(); }
		size_t size()		const { return _span.size(); }

		Span<const u8> span() const		{ return _span; }
		operator	Span<const u8>()	{ return _span; }

		const String& filename() const { return _fs.filename(); }

	private:
		FileStream _fs;
		Span<const u8> _span;

#if  SGE_OS_WINDOWS
		::HANDLE _mapping = nullptr;
#endif //  SGE_OS_WINDOWS

	};
}