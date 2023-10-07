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

void AssimpNameSpace::LoadGeometry(std::string& path_name, std::vector<Meshes>& MeshVector)
{
	const aiScene* scene = aiImportFile(path_name.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path_name);
}