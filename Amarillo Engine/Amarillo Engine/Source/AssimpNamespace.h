#pragma once
#include "Application.h"
#include "Globals.h"

#include "..\External\Assimp/include/cimport.h"
#include "..\External\Assimp/include/scene.h"
#include "..\External\Assimp/include/postprocess.h"



#pragma comment (lib, "External/Assimp/libx86/assimp.lib")


struct Meshes
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

class AssimpNameSpace
{
public:

	void EnableDebug();
	void DisableDebug();

	static void LoadGeometry(const char* path_name, std::vector<Meshes>& MeshVector);
};