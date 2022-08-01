#include "Terrain.h"

namespace sge
{

	void Terrain::CreateEditMesh(int _length, int _width)
	{
		int xSize = _width;
		int zSize = _length;
		GridXSize = xSize * patchsize;
		GridZSize = zSize * patchsize;
		for (int z = 0; z <= GridZSize; z++)
		{
			for (int x = 0; x <= GridXSize; x++)
			{
				float xPos = 0.25f * x;
				float zPos = 0.25f * z;
				_vertex.emplace_back(xPos, zPos, 0);
			}
		}
		
		int vert = 0;
		int tris = 0;

		_patches.resize(xSize * zSize);
		auto* p = _patches.begin();

		for (int z = 0; z < zSize; z++)
		{
			for (int x = 0; x < xSize; x++)
			{
				p->create(this, Vec2i(x, z));
				p++;

				vert += 4;
			}
			vert = (GridXSize + 1) * (4 * (z + 1));
		}
	}
	void Terrain::emplaceVertex(const Vector<int> &vertexIndex)
	{
		auto* v = vertexIndex.begin();
		while (v != vertexIndex.end())
		{
			_terrainMesh.pos.emplace_back(_vertex[*v]);
			_terrainMesh.color.emplace_back(255, 255, 255, 255);
			_terrainMesh.uv[0].emplace_back(0, 0);
			_terrainMesh.normal.emplace_back(0.0f, 0.0f, 1.0f);
			v++;
		}
		
	}
	void Terrain::Patch::create(Terrain* terrain, const Vec2i& pos)
	{
		auto* t = _triangles.begin();
		for (int i = 0; i < 4; i++)
		{
			t->create(terrain, pos, i);
			t++;
		}
		SGE_LOG("Create Patch {}, {}", pos.x, pos.y);

	}
	void Terrain::GridTriangle::create(Terrain* terrain, const Vec2i& pos, int index)
	{
		triangleIndex.resize(3);
		int patchSize = terrain->patchsize;
		int terrainSize = terrain->GridXSize;

		int x = patchSize * pos.x + (terrainSize * 4 + 4) * pos.y;

		switch (index)
		{
		case 0:
			triangleIndex.emplace_back(x);
			triangleIndex.emplace_back(x + terrainSize * 2 + 4);
			triangleIndex.emplace_back(x + 4);
			break;
		case 1:
			triangleIndex.emplace_back(x + 4);
			triangleIndex.emplace_back(x + terrainSize * 2 + 4);
			triangleIndex.emplace_back(x + terrainSize * 4 + 8);
			break;
		case 2:
			triangleIndex.emplace_back(x + terrainSize * 4 + 8);
			triangleIndex.emplace_back(x + terrainSize * 2 + 4);
			triangleIndex.emplace_back(x + terrainSize * 4 + 4);
			break;
		case 3:
			triangleIndex.emplace_back(x + terrainSize * 4 + 4);
			triangleIndex.emplace_back(x + terrainSize * 2 + 4);
			triangleIndex.emplace_back(x);
			break;
		default:
			break;
		}

		terrain->emplaceVertex(triangleIndex);

	}
}
