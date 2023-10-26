#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "../External/ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	//parent->AddComponent(this);
	mesh = nullptr;
	path = "No path!";
	type = ComponentTypes::MESH;
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::OnEditor() {

	if (ImGui::CollapsingHeader("Component Mesh"))
	{
		if (mesh != NULL) {
			ImGui::Text("Path: %s", path);
			ImGui::Text("Index: %d", mesh->num_index);
			ImGui::Text("Vertices: %d", mesh->num_vertex);
		}
		//Todo:Falta poner estas dos

		/*ImGui::Text("Normals: %d", mesh->);
		ImGui::Text("texture: %d", mesh->);*/
	}
}



void ComponentMesh::SetMesh(Meshes* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}

void ComponentMesh::SetVertices(const aiVector3D* vertices, unsigned int numVertices) {
	// You should store the vertex data in your ComponentMesh
	// For example, you might store it in a member variable or use a rendering library to upload the data to the GPU.

	// Here's a simplified example of how to store vertex data in a member variable:
	for (unsigned int i = 0; i < numVertices; ++i) {
		// Store vertices in your mesh data structure
		// For simplicity, we assume that you have a "vertices" member in ComponentMesh
		vertices_.push_back(vec3(vertices[i].x, vertices[i].y, vertices[i].z));
	}
}