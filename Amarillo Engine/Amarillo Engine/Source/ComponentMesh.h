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
	ComponentMesh(GameObject* parent);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetMesh(ModuleMesh::Mesh* mesh);
	void SetPath(std::string path);


};
