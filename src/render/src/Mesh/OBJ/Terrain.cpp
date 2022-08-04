#include "Terrain.h"

namespace sge
{

	void Terrain::CreateEditMesh(int _length, int _width)
	{
		int xSize = _width;
		int zSize = _length;
		int totalSize = xSize * zSize;
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

		_patches.resize(totalSize);
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


		p = _patches.begin();
		auto* dir_p = _patches.begin();

		for (int i = 0; i < totalSize; i++)
		{
			int curX = i;
			int x = curX % xSize - 1;
		
			if (x >= 0)
			{
				dir_p = p - 1;
				if (p->LOD > dir_p->LOD)
				{
					p->subdivision(3);
				}

			}
			x = curX % xSize + 1;
			if (x < xSize)
			{
				dir_p = p + 1;
				if (p->LOD > dir_p->LOD)
				{
					p->subdivision(1);
				}
			}
			int y = curX + xSize;
			if (y < totalSize)
			{
				dir_p = p + xSize;
				if (p->LOD > dir_p->LOD)
				{
					
					p->subdivision(2);
				}
			}
			y = curX - xSize;
			if (y >= 0)
			{
				dir_p = p - xSize;
				if (p->LOD > dir_p->LOD)
				{
					p->subdivision(0);
				}
			}
			p++;
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
		float testLOD = pos.x + pos.y;

		if (testLOD < 4)
			LOD = 0;
		else if (testLOD < 8)
			LOD = 1;
		else
			LOD = 2;

		for (int i = 0; i < 4; i++)
		{
			t->create(terrain, pos, LOD, i);
			t++;
		}
		SGE_LOG("Create Patch {}, {}", pos.x, pos.y);
	}

	void Terrain::Patch::subdivision(int dir)
	{
		auto* t = _triangles.begin();
		t += dir;
		t->subdivision();
	}


	void Terrain::GridTriangle::create(Terrain* terrain, const Vec2i& pos, int patchLOD, int index)
	{
		_patchLOD = patchLOD;
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
			if (_patchLOD == 2)
			{
				triangleIndex.emplace_back(v0);
				triangleIndex.emplace_back(center);
				triangleIndex.emplace_back(v1);
				_subTriangle.emplace_back(Vec3i{ v0 ,center, v1 });
			}
			else
				subdivision(v0, center, v1, _patchLOD);
			break;

		case 1:		
			if (_patchLOD == 2)
			{
				triangleIndex.emplace_back(v1);
				triangleIndex.emplace_back(center);
				triangleIndex.emplace_back(v2);
				_subTriangle.emplace_back(Vec3i{ v1 ,center, v2 });
			}
			else
				subdivision(v1, center, v2, _patchLOD);
		case 2:
			if (_patchLOD == 2)
			{
				triangleIndex.emplace_back(v2);
				triangleIndex.emplace_back(center);
				triangleIndex.emplace_back(v3);
				_subTriangle.emplace_back(Vec3i{ v2 ,center, v3 });
			}
			else
				subdivision(v2, center, v3, _patchLOD);
			break;
		case 3:
			if (_patchLOD == 2)
			{
				triangleIndex.emplace_back(v3);
				triangleIndex.emplace_back(center);
				triangleIndex.emplace_back(v0);
				_subTriangle.emplace_back(Vec3i{ v3 ,center, v0 });
			}
			else
				subdivision(v3, center, v0, _patchLOD);
			break;
		default:
			break;
		}

		terrain->emplaceVertex(triangleIndex);

	}
	void Terrain::GridTriangle::subdivision(int _v0, int _v1, int _v2, int _sdCount)
	{

		int remainLOD = _terrain->MaxLOD - _sdCount;

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
	void Terrain::GridTriangle::subdivision(int _v0, int _v1, int _v2)
	{

		int v0 = _v0;
		int v1 = _v1;
		int v2 = _v2;
		int center = (_v0 + _v2) / 2;


		triangleIndex.emplace_back(v1);
		triangleIndex.emplace_back(center);
		triangleIndex.emplace_back(v0);

		triangleIndex.emplace_back(v2);
		triangleIndex.emplace_back(center);
		triangleIndex.emplace_back(v1);

		_terrain->emplaceVertex(triangleIndex);
	}
	void Terrain::GridTriangle::subdivision()
	{
		auto v = _subTriangle.begin();
		for (int i = 0; i < _subTriangle.size(); i++)
		{
			subdivision(v->x, v->y, v->z);
			v++;
		}
	}
}