#pragma once
#include "Component.h"
#include "glmath.h"
#include "ModuleMesh.h"


class ComponentMesh : public Component {
	//Properties
public:

public:
	ModuleMesh::Mesh* mesh;
	std::string paths;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetMesh(ModuleMesh::Mesh* mesh);
	void SetPath(std::string path);

	void InitBoundingBoxes(ModuleMesh::Mesh* mesh);

	void UpdateBoundingBoxes(std::vector<GameObject*> gameobjects);

	void RenderBoundingBoxes();

	ComponentMesh(GameObject* parent);
public:
	AABB globalAABB;

	AABB aabb;
	OBB obb;



};
