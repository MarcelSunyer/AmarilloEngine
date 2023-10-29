#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Source/Module.h"
#include "Source/Globals.h"
#include "Source/glmath.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

class ModuleMesh : public Module
{
public:
	ModuleMesh(Application* app, bool start_enabled = true);

	void LoadMesh(const char* file_path);

	~ModuleMesh();
	update_status Update();
	bool CleanUp();
	struct Vertex {
		float3 Position;
		float3 Normal;
		float2 TexCoords;
	};

	struct MeshData {
		std::vector<unsigned int> indices;
		std::vector<Vertex> ourVertex;
		unsigned int VBO = 0, EBO = 0;
	};
	std::vector<MeshData> ourMeshes;

};






#endif //MODULE_IMPORTER
