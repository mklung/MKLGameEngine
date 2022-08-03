#pragma once

#include "Mesh/EditMesh.h"

namespace sge
{
#define PatchDirection_ENUM_LIST(E) \
	E(None, = 0) \
	E(North, = 1 << 0)  \
	E(East,  = 1 << 1)  \
	E(South, = 1 << 2)  \
	E(West,  = 1 << 3)  \
//----
SGE_ENUM_CLASS(PatchDirection, u8);
SGE_ENUM_ALL_OPERATOR(PatchDirection)


	class Terrain : public NonCopyable
	{
	public:
		const int patchsize = 4;
		const int MaxLOD = 3;

		void CreateEditMesh(int length, int width);
		EditMesh* getTerrainMesh() { return &_terrainMesh; }

		class GridTriangle
		{
		public:
			void create(Terrain* terrain, const Vec2i& pos, int _LOD, int index);
			void subdivision(int _v0, int _v1, int _v2, int _lodremain);
			void subdivision(int _v0, int _v1, int _v2, int _lodremain, int LorR);
			void subdivision(int _v0, int _v1, int _v2);
		private:
			int				_patchLOD;
			int				_lod;
			Vector<int>		triangleIndex;
			Terrain*		_terrain = nullptr;
			Vector<Vec3i>	_subTriangle;

		};

		class Patch
		{
		public:
			int LOD = 0;
			void create(Terrain* terrain, const Vec2i& pos);
			
		private:
			Terrain*					_terrain = nullptr;
			Vec2i						_pos{ 0, 0 };
			Vector_<GridTriangle, 4>	_triangles;
		};

	const	Vector<Vec3f>&	terrainVertex() const { return _vertex; }
			void			emplaceVertex(const Vector<int> &vertexIndex);

	private:
		int				GridXSize = 0;
		int				GridZSize = 0;
		EditMesh		_terrainMesh;
		Vec3f			_cameraPos{ 0,0,0 };
		Vector<Vec3f>	_vertex;
		Vector<Patch>	_patches;
	};


}