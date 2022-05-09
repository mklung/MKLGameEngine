#pragma once

#include "Mesh/EditMesh.h"

namespace sge
{
	class ObjLoader
	{
	public:
		static void LoadFile(EditMesh& mesh, StrView filename);
		static void LoadMem(EditMesh& mesh, Span<const u8> src);


		static void abc();
	private:
		void _loadmem(EditMesh& mesh, Span<const u8> src);

		void _nextLine();
		void _nextToken();
		void _parseLine();
		void _parseLine_v();
		void _parseLine_vt();
		void _parseLine_vn();
		void _parseLine_f();

		void _error(StrView msg) { throw SGE_ERROR("{}: {}", _lineNumber, msg); }

		EditMesh* _outMesh = nullptr;

		StrView	_source;
		StrView	_token;
		StrView	_currentLine;
		StrView	_lineRemain;
		StrView	_sourceRemain;

		int _lineNumber = 0;

		Vector<Tuple3f> _tmpPos;
		Vector<Tuple2f> _tmpUv;
		Vector<Tuple3f> _tmpNormal;

	};
}