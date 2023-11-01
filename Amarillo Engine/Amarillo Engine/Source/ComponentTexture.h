#pragma once
#include "Component.h"
#include "glmath.h"
#include "ModuleMesh.h"


class ComponentTexture : public Component {
	//Properties
public:

private:
	ModuleMesh::MeshData mesh;
	std::string paths;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void setTexture(ModuleMesh::MeshData mesh);
	void SetPath(std::string path);


	ComponentTexture(GameObject* parent);
private:

};
