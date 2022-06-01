#pragma once 

namespace sge
{
	struct  Directory
	{
	public:
		Directory() = delete;

		static void setCurrent(StrView dir);
		static String getCurrent();
		static void create(StrView path);

		static bool exist(StrView path);

	private:
		static void _create(StrView path);

	};
}