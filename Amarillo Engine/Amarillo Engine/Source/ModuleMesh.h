#ifndef MODULE_MESH
#define MODULE_MESH

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "../External/MathGeoLib/include/Math/float3x3.h"
#include "../External/MathGeoLib/include/MathGeoLib.h"

#include "../External/Assimp/include/cimport.h"
#include "../External/Assimp/include/scene.h"
#include <vector>

#include "../External/Glew/include/glew.h"

class GameObject;

class ModuleMesh : public Module
{
public:
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
	ModuleMesh(Application* app, bool start_enabled = true);

	GameObject* LoadMesh(const char* file_path);

	~ModuleMesh();

	bool Init() override;
	update_status Update(float dt);
	bool CleanUp();
	void DrawNormals();
	void GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject);
	Mesh ProcessMesh(aiMesh* Mesh, const char* file_path, GameObject* gameObject);

	

	
	std::vector<Mesh*> ourMeshes;
	std::string name = "EmptyObject_";
	int num = 0;
public:
//	void InitBoundingBoxes();
//
//	void UpdateBoundingBoxes();
//
//	void RenderBoundingBoxes();
//public:
//	AABB globalAABB;
//	float scale_factor = 1.0f;
//
//	AABB aabb;
//	OBB obb;

};

#endif //MODULE_MESH
