#pragma once
#include "Component.h"
#include "glmath.h"
#include "AssimpNamespace.h"


class ComponentMesh : public Component {
	//Properties
public:

private:
	aiMesh* mesh;
	std::string paths;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;

	void SetMesh(aiMesh* mesh);
	void SetPath(std::string path);


	ComponentMesh(GameObject* parent);
private:

};
