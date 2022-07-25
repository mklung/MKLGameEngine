#include "Terrain.h"

namespace sge
{

	void Terrain::CreateEditMesh(int _length, int _width)
	{
		float xSize = _width;
		float zSize = _length;
		for (int z = 0; z <= zSize; z++)
		{
			for (int x = 0; x <= xSize; x++)
			{
				float xPos = 0.25f * x;
				float zPos = 0.25f * z;
				vertex.emplace_back(xPos, zPos, 0);
			}
		}
		
		int vert = 0;
		int tris = 0;
		for (int z = 0; z < zSize; z++)
			for (int x = 0; x < xSize; x++)
			{
				_terrainMesh.pos.emplace_back(vertex[vert + 0]);
				_terrainMesh.pos.emplace_back(vertex[vert + xSize + 1]);
				_terrainMesh.pos.emplace_back(vertex[vert + 1]);
				_terrainMesh.pos.emplace_back(vertex[vert + 1]);
				_terrainMesh.pos.emplace_back(vertex[vert + xSize + 1]);
				_terrainMesh.pos.emplace_back(vertex[vert + xSize + 2]);

				_terrainMesh.color.emplace_back(255, 255, 255, 255);
				_terrainMesh.color.emplace_back(255, 255, 255, 255);
				_terrainMesh.color.emplace_back(255, 255, 255, 255);
				_terrainMesh.color.emplace_back(255, 255, 255, 255);
				_terrainMesh.color.emplace_back(255, 255, 255, 255);
				_terrainMesh.color.emplace_back(255, 255, 255, 255);

				_terrainMesh.uv[0].emplace_back(0, 0);
				_terrainMesh.uv[0].emplace_back(0, 0);
				_terrainMesh.uv[0].emplace_back(0, 0);				
				_terrainMesh.uv[0].emplace_back(0, 0);
				_terrainMesh.uv[0].emplace_back(0, 0);
				_terrainMesh.uv[0].emplace_back(0, 0);

				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
				_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);

				vert++;
			}
		
	}
}
