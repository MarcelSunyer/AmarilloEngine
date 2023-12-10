#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(uuids::uuid unique_id) : Resource(unique_id, ResourceType::RT_TEXTURE)
{

}

ResourceMesh::~ResourceMesh()
{
}

void ChildMesh::CleanUp()
{
	UnloadFromMemory();

	id_vertices = 0;
	num_vertices = 0;
	RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	RELEASE_ARRAY(indices);

	// UVs
	id_uv = 0;
	num_uvs = 0;
	RELEASE_ARRAY(uvs);
}

void ChildMesh::SetFaces(float* _vertices, uint _num_vertices, uint* _indices, uint _num_indices)
{
	vertices = _vertices;
	num_vertices = _num_vertices;
	indices = _indices;
	num_indices = _num_indices;
}

void ChildMesh::SetUvs(float* _uvs, uint _num_uvs)
{
	uvs = _uvs;
	num_uvs = _num_uvs;
}

void ChildMesh::SetTransform(float3 _pos, Quat _rotation, float3 _scale)
{
	position = _pos;
	rotation = _rotation;
	scale = _scale;
}

uint ChildMesh::GetIdVertices()
{
	return id_vertices;
}

uint ChildMesh::GetNumVertices()
{
	return num_vertices;
}

uint ChildMesh::GetIdIndices()
{
	return id_indices;
}

uint ChildMesh::GetNumIndices()
{
	return num_indices;
}

uint ChildMesh::GetIdUV()
{
	return id_uv;
}

uint ChildMesh::GetNumUVs()
{
	return num_uvs;
}

float* ChildMesh::GetVertices()
{
	return vertices;
}

uint* ChildMesh::GetIndices()
{
	return indices;
}

float* ChildMesh::GetUVs()
{
	return uvs;
}

AABB ChildMesh::GetBBox()
{
	return bbox;
}

float ChildMesh::GetDiagonal()
{
	return bbox.Diagonal().Length();
}

float3 ChildMesh::GetPosition()
{
	return position;
}

Quat ChildMesh::GetRotation()
{
	return rotation;
}

float3 ChildMesh::GetScale()
{
	return scale;
}
void ChildMesh::LoadToMemory()
{
	if (id_vertices == 0 && vertices != nullptr)
		id_vertices = applic->renderer3D->LoadBuffer(vertices, num_vertices * 3);

	if (id_indices == 0 && indices != nullptr)
		id_indices = applic->renderer3D->LoadBuffer(indices, num_indices);

	if (id_uv == 0 && uvs != nullptr)
		id_uv = applic->renderer3D->LoadBuffer(uvs, num_uvs * 3);
}

void ChildMesh::UnloadFromMemory()
{
	if (id_vertices != 0)
	{
		applic->renderer3D->UnloadBuffer(id_vertices, num_vertices * 3);
		id_vertices = 0;
	}

	if (id_indices != 0)
	{
		applic->renderer3D->UnloadBuffer(id_indices, num_indices);
		id_indices = 0;
	}

	if (id_uv != 0)
	{
		applic->renderer3D->UnloadBuffer(id_uv, num_uvs * 3);
		id_uv = 0;
	}
}

