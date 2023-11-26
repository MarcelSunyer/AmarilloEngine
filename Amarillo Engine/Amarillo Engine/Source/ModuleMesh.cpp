#include "ModuleMesh.h"
#include "Application.h"
#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include "../External/Assimp/include/postprocess.h"
#include <vector>
#include "GameObject.h"
#include "ModuleRenderer3D.h"
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

bool ModuleMesh::Init()
{
	return true;
}

update_status ModuleMesh::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}

GameObject* ModuleMesh::LoadMesh(const char* file_path)
{
	const aiScene* imported_scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (imported_scene->HasMeshes() && imported_scene != nullptr)
	{
		GameObject* parentObject = App->scene->CreateGameObject("GameObject", App->scene->root_object);

		GetSceneInfo(imported_scene->mRootNode, imported_scene, file_path, parentObject);

		aiReleaseImport(imported_scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}

	return newMesh;
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

void ModuleMesh::GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{

		GetSceneInfo(node->mChildren[i], scene, file_path, gameObject);
	}

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], file_path, gameObject);
	}
}

ModuleMesh::Mesh ModuleMesh::ProcessMesh(aiMesh* mesh, const char* file_path, GameObject* gameObject)
{
	Mesh* myMesh = new Mesh();
	
	newMesh = App->scene->CreateGameObject(file_path, gameObject);

	for (unsigned int j = 0; j < mesh->mNumVertices; j++)
	{
		Vertex vertex;
		float3 vector;
		vector.x = mesh->mVertices[j].x;
		vector.y = mesh->mVertices[j].y;
		vector.z = mesh->mVertices[j].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[j].x;
			vertex.Normal.y = mesh->mNormals[j].y;
			vertex.Normal.z = mesh->mNormals[j].z;

		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][j].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][j].y;
		}
		else
		{
			vertex.TexCoords.x = 0.0f;
			vertex.TexCoords.y = 0.0f;
		}

		myMesh->ourVertex.push_back(vertex);
	}

	if (mesh->HasFaces())
	{
		myMesh->indices.resize(mesh->mNumFaces * 3);	//assume each face is a triangle

		for (uint y = 0; y < mesh->mNumFaces; y++)
		{
			if (mesh->mFaces[y].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&myMesh->indices[y * 3], mesh->mFaces[y].mIndices, 3 * sizeof(unsigned int));
			}
		}
	}

	ComponentMesh* mesh_component = (ComponentMesh*)newMesh->AddComponent(ComponentTypes::MESH);
	mesh_component->SetMesh(myMesh);
	mesh_component->SetPath(file_path);

	ourMeshes.push_back(myMesh);

	return *myMesh;
}

//void ModuleMesh::InitBoundingBoxes()
//{
//	obb.SetNegativeInfinity();
//	globalAABB.SetNegativeInfinity();
//
//	for (const auto& mesh : ourMeshes) {
//		std::vector<float3> floatArray;
//		floatArray.reserve(mesh->ourVertex.size());
//
//		for (const auto& vertex : mesh->ourVertex) {
//			floatArray.push_back(vertex.Position);
//		}
//
//		aabb.SetFrom(&floatArray[0], floatArray.size());
//	}
//}
//
//
//void ModuleMesh::UpdateBoundingBoxes()
//{
//	for (const auto& gameobject : App->scene->game_objects)
//	{
//		if (gameobject != nullptr && gameobject->transform != nullptr)
//		{	
//			obb = aabb;
//			obb.Transform(gameobject->transform->transform);
//
//			globalAABB.SetNegativeInfinity();
//			globalAABB.Enclose(obb);
//			RenderBoundingBoxes();
//		}
//		else
//		{
//			LOG("Error: GameObject or its transform is null");
//		}
//		
//	}
//}
//
//void ModuleMesh::RenderBoundingBoxes()
//{
//	float3 verticesO[8];
//	obb.GetCornerPoints(verticesO);
//	App->renderer3D->DrawBoundingBox(verticesO, float3(0, 255, 0));
//
//	float3 verticesA[8];
//	globalAABB.GetCornerPoints(verticesA);
//	App->renderer3D->DrawBoundingBox(verticesA, float3(0, 255, 0));
//}
//

