#ifndef MODULE_MESH
#define MODULE_MESH

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"
#include <vector>
#include "../External/Glew/include/glew.h"

class ModuleMesh : public Module
{
public:
	ModuleMesh(Application* app, bool start_enabled = true);

	void LoadMesh(const char* file_path);

	~ModuleMesh();
	update_status Update();
	bool CleanUp();
	void DrawNormals();

	struct Vertex {
		float3 Position;
		float3 Normal;
		float2 TexCoords;
	};

	struct Mesh {
		std::vector<unsigned int> indices;
		std::vector<Vertex> ourVertex;
		unsigned int VBO = 0, EBO = 0;
	};
	std::vector<Mesh*> ourMeshes;
	std::string name = "EmptyObject_";
	int num = 0;
};

#endif //MODULE_MESH
