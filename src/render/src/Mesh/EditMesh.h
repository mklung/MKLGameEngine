#pragma once

#include "../Vertex/Vertex.h"

namespace sge {

	class EditMesh : public NonCopyable
	{
	public:
		static const u8 kUvCountMax = 4;

		Vector<Tuple3f> pos;
		Vector<Color4b> color;

		Vector<Tuple2f>	uv[kUvCountMax];

		Vector<Tuple3f>	normal;
		Vector<Tuple3f>	tangent;
		Vector<Tuple3f>	binormal;

	private:

	};

}