#pragma once
#include "Component.h"
#include "glmath.h"
#include "AssimpNamespace.h"


class ComponentMesh : public Component {
	//Properties
public:

private:
	Meshes* mesh;
	std::string path;

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor() override;
	Meshes* GetMesh() const { return mesh; };
	std::string  GetPath() const { return path; };

	void SetMesh(Meshes* mesh);
	void SetPath(std::string path);


	ComponentMesh(GameObject* parent);
public:
	std::vector<vec3> vertices_;
};
