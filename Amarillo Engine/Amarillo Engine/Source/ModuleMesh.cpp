#include "ModuleMesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

void ModuleMesh::LoadMesh(const char* file_path)
{

	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) {

			MeshData temp;


			for (unsigned int o = 0; o < scene->mMeshes[i]->mNumVertices; o++)
			{
				Vertex tempvertex;
				float3 vector;
				vector.x = scene->mMeshes[i]->mVertices[o].x;
				vector.y = scene->mMeshes[i]->mVertices[o].y;
				vector.z = scene->mMeshes[i]->mVertices[o].z;
				tempvertex.Position = vector;
				LOG("New mesh with %d vertices", scene->mMeshes[i]->mNumVertices);

				if (scene->mMeshes[i]->HasNormals())
				{
					tempvertex.Normal.x = scene->mMeshes[i]->mNormals[o].x;
					tempvertex.Normal.y = scene->mMeshes[i]->mNormals[o].y;
					tempvertex.Normal.z = scene->mMeshes[i]->mNormals[o].z;

				}
				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					tempvertex.TexCoords.x = scene->mMeshes[i]->mTextureCoords[0][o].x;
					tempvertex.TexCoords.y = scene->mMeshes[i]->mTextureCoords[0][o].y;

				}
				else
				{
					tempvertex.TexCoords.x = 0.0f;
					tempvertex.TexCoords.y = 0.0f;
				}

				temp.ourVertex.push_back(tempvertex);
			}


			if (scene->mMeshes[i]->HasFaces())
			{

				temp.indices.resize(scene->mMeshes[i]->mNumFaces * 3);// assume each face is a triangle

				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {

						memcpy(&temp.indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));

					}
				}
			}
			ourMeshes.push_back(temp);
		}


		aiReleaseImport(scene);


	}
	else
		LOG("Error loading scene % s", file_path);
}

ModuleMesh::~ModuleMesh()
{
}

update_status Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}