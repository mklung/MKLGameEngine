#pragma once

#include "Mesh/EditMesh.h"

namespace sge
{
	class Terrain : public NonCopyable
	{
	public:
		void CreateEditMesh(int length, int width);

		EditMesh* getTerrainMesh() { return &_terrainMesh; }
	private:
		EditMesh _terrainMesh;
		Vector<Tuple3f> vertex;
	};
}