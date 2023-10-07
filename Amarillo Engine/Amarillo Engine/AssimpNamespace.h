#pragma once
#include "Application.h"
#include "Globals.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")


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

	void LoadGeometry(std::string& path_name, std::vector<Meshes>& MeshVector);
};