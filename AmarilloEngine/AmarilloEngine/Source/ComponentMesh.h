#pragma once
#include "Component.h"
#include "ModuleMesh.h"

#include "../AmarilloEngine/External/MathGeoLib/include/Geometry/AABB.h"
#include "../AmarilloEngine/External/MathGeoLib/include/Geometry/OBB.h"

class ComponentMesh : public Component {

public:
	ModuleMesh::Mesh* mesh_;
	std::string paths;
	AABB globalAABB;
	float scale_factor = 1.0f;

	AABB aabb;
	OBB obb;

	//Methods
public:
	ComponentMesh(GameObject* parent);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;
	void DebugDraw() override;

	void SetMesh(ModuleMesh::Mesh* mesh);
	void SetPath(std::string path);

	void InitBoundingBoxes();

	void UpdateBoundingBoxes();

	void RenderBoundingBoxes();

};
