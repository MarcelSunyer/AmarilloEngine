#pragma once

#include "Globals.h"

#include "ModuleMesh.h"
#include "../External/Assimp/include/mesh.h"
#include <fstream>

namespace CFF_Mesh {

	void Import(const aiMesh* mesh, ModuleMesh::Mesh* ourMesh); // Import Mesh from Assimp
	uint Save(const ModuleMesh::Mesh* ourMesh, char** fileBuffer); // Save Mesh as .ymesh on Library
	void Load(const char* fileBuffer, ModuleMesh::Mesh* ourMesh); // Load Mesh from Library

	bool SaveMeshToFile(const ModuleMesh::Mesh* ourMesh, const std::string& filename);

}