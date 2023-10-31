#pragma once
#include "Component.h"
#include "glmath.h"
#include "ModuleMesh.h"


class ComponentMesh : public Component {
	//Properties
public:

private:
	MeshData* mesh;
	std::string paths;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetMesh(MeshData* mesh);
	void SetPath(std::string path);


	ComponentMesh(GameObject* parent);
private:

};
