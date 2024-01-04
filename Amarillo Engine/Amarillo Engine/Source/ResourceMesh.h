#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"
#include "Globals.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Geometry/AABB.h"
#include "../External/MathGeoLib/include/Math/Quat.h"
#include "ModuleRenderer3D.h"

class ChildMesh
{
public:
	void CleanUp();

	void SetFaces(float* _vertices, uint _num_vertices, uint* _indices, uint _num_indices);
	void SetUvs(float* _uvs, uint _num_uvs);
	void SetTransform(float3 _pos, Quat _rotation, float3 _scale);

	uint GetIdVertices();
	uint GetNumVertices();
	uint GetIdIndices();
	uint GetNumIndices();
	uint GetIdUV();
	uint GetNumUVs();
	float* GetVertices();
	uint* GetIndices();
	float* GetUVs();
	AABB GetBBox();
	float GetDiagonal();

	float3 GetPosition();
	Quat   GetRotation();
	float3 GetScale();

private:
	void LoadToMemory();
	void UnloadFromMemory();

private:
	uint   id_vertices = 0;
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0;
	uint   num_indices = 0;
	uint* indices = nullptr;

	uint   id_uv = 0;
	uint   num_uvs = 0;
	float* uvs = nullptr;

	float3 position = float3::zero;
	Quat   rotation = Quat::identity;
	float3 scale = float3(1, 1, 1);

	AABB   bbox;
};

class __declspec(dllexport) ResourceMesh : public Resource
{
public:
	ResourceMesh(uuids::uuid unique_id);
	virtual ~ResourceMesh();
	
	std::vector<ChildMesh*> childs;
};

#endif