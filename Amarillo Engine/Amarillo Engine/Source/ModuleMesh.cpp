#include "ModuleMesh.h"
#include "Application.h"
#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"
#include <vector>
#include "ModuleScene.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "GameObject.h"
#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleMesh::~ModuleMesh()
{
}

update_status ModuleMesh::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}

void ModuleMesh::LoadMesh(const char* file_path)
{
	const aiScene* imported_scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//Editar esto para que no se llamenigual pq sino da error
	App->scene->root_object = App->scene->CreateGameObject(name + std::to_string(num));
	num += 1;

	if (imported_scene->HasMeshes() && imported_scene != nullptr)
	{
		for (int i = 0; i < imported_scene->mNumMeshes; i++)
		{
			Mesh* mesh_obj = new Mesh();

			for (unsigned int o = 0; o < imported_scene->mMeshes[i]->mNumVertices; o++)
			{
				Vertex vertex_data;
				float3 vertex_position;
				vertex_position.x = imported_scene->mMeshes[i]->mVertices[o].x;
				vertex_position.y = imported_scene->mMeshes[i]->mVertices[o].y;
				vertex_position.z = imported_scene->mMeshes[i]->mVertices[o].z;
				vertex_data.Position = vertex_position;
				LOG("New mesh with %d vertices", imported_scene->mMeshes[i]->mNumVertices);

				if (imported_scene->mMeshes[i]->HasNormals())
				{
					vertex_data.Normal.x = imported_scene->mMeshes[i]->mNormals[o].x;
					vertex_data.Normal.y = imported_scene->mMeshes[i]->mNormals[o].y;
					vertex_data.Normal.z = imported_scene->mMeshes[i]->mNormals[o].z;
				}

				if (imported_scene->mMeshes[i]->HasTextureCoords(0))
				{
					vertex_data.TexCoords.x = imported_scene->mMeshes[i]->mTextureCoords[0][o].x;
					vertex_data.TexCoords.y = imported_scene->mMeshes[i]->mTextureCoords[0][o].y;
				}
				else
				{
					vertex_data.TexCoords.x = 0.0f;
					vertex_data.TexCoords.y = 0.0f;
				}

				mesh_obj->ourVertex.push_back(vertex_data);
			}

			if (imported_scene->mMeshes[i]->HasFaces())
			{
				mesh_obj->indices.resize(imported_scene->mMeshes[i]->mNumFaces * 3);

				for (uint y = 0; y < imported_scene->mMeshes[i]->mNumFaces; y++)
				{
					if (imported_scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh_obj->indices[y * 3], imported_scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));
					}
				}
			}

			ComponentMesh* mesh_component = (ComponentMesh*)App->scene->root_object->AddComponent(ComponentTypes::MESH);
			mesh_component->SetMesh(mesh_obj);
			mesh_component->SetPath(file_path);
			ourMeshes.push_back(mesh_obj);
		}

		aiReleaseImport(imported_scene);
	}
	else
		LOG("Error loading scene % s", file_path);
}



void ModuleMesh::DrawNormals() {
	float length = 0.2f; // Define the length of the normals you want to draw

	glBegin(GL_LINES); // Start drawing lines

	for (Mesh* mesh : ourMeshes) {
		for (unsigned int i = 0; i < mesh->indices.size(); i += 3) {
			// Get the vertices of the triangle
			float3 vertex1 = mesh->ourVertex[mesh->indices[i]].Position;
			float3 vertex2 = mesh->ourVertex[mesh->indices[i + 1]].Position;
			float3 vertex3 = mesh->ourVertex[mesh->indices[i + 2]].Position;

			// Calculate the center of the triangle
			float3 center = (vertex1 + vertex2 + vertex3) / 3.0f;

			// Get the normal of the center (this assumes the normals are the same for all vertices in the triangle)
			float3 normal = mesh->ourVertex[mesh->indices[i]].Normal;

			// Calculate the end point of the normal
			float3 normalEnd = center + normal * length;

			// Set the color of the line (optional)
			glColor3f(1.0f, 1.0f, 0.0f); // Red color

			// Define the start and end points of the line
			glVertex3f(center.x, center.y, center.z);
			glVertex3f(normalEnd.x, normalEnd.y, normalEnd.z);
		}
	}

	glEnd(); // End drawing lines
}


