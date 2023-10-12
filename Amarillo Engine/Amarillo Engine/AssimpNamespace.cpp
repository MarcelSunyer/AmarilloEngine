#include "AssimpNameSpace.h"


void AssimpNameSpace::EnableDebug()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void AssimpNameSpace::DisableDebug()
{
	// detach log stream
	aiDetachAllLogStreams();
}

void AssimpNameSpace::LoadGeometry(const char* path_name, std::vector<Meshes>& MeshVector)
{
	const aiScene* scene = aiImportFile(path_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		// Itera a través de todos los meshes
		
		// Reservar espacio para las mallas en el vector
		MeshVector.resize(scene->mNumMeshes);

		// Iterar sobre todas las meshes en scene
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* aiMesh = scene->mMeshes[i];
			Meshes& ourMesh = MeshVector[i];

			// copy vertices
			ourMesh.num_vertex = aiMesh->mNumVertices;
			ourMesh.vertex = new float[ourMesh.num_vertex * 3];
			memcpy(ourMesh.vertex, aiMesh->mVertices, sizeof(float) * ourMesh.num_vertex * 3);
			LOG("New mesh with %d vertices", ourMesh.num_vertex);
			
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path_name);
	}
		
}
