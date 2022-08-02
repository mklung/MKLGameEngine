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
			t->create(terrain, pos, LOD, i);
			t++;
		}
		SGE_LOG("Create Patch {}, {}", pos.x, pos.y);

	}
	void Terrain::GridTriangle::create(Terrain* terrain, const Vec2i& pos, int patchLOD, int index)
	{
		_patchLOD = patchLOD;
		//triangleIndex.resize(3);
		_terrain = terrain;
		int patchSize = _terrain->patchsize;
		int terrainSize = _terrain->GridXSize;

		int x = patchSize * pos.x + (terrainSize * 4 + 4) * pos.y;

		int v0 = x;
		int v1 = x + 4;
		int v2 = x + terrainSize * 4 + 8;
		int v3 = x + terrainSize * 4 + 4;

		int center = x + terrainSize * 2 + 4;

		switch (index)
		{
		case 0:
			subdivision(v0, center, v1, 0);
			break;
		case 1:
			subdivision(v1, center, v2, 0);
			break;
		case 2:
			subdivision(v2, center, v3, 0);
			break;
		case 3:
			subdivision(v3, center, v0, 0);
			break;
		default:
			break;
		}

		terrain->emplaceVertex(triangleIndex);

	}
	void Terrain::GridTriangle::subdivision(int _v0, int _v1, int _v2, int _sdCount)
	{

		int remainLOD = _terrain->MaxLOD - _sdCount;
		int a = 0;
		if (remainLOD == 0)
		{
			triangleIndex.emplace_back(_v0);
			triangleIndex.emplace_back(_v1);
			triangleIndex.emplace_back(_v2);
			return;
		}
		else
		{
			int v0 = _v0;
			int v1 = _v1;
			int v2 = _v2;
			int center = (_v0 + _v2) / 2;
			subdivision(v1, center, v0, _sdCount + 1);
			subdivision(v2, center, v1, _sdCount + 1);

		}

	}
}
