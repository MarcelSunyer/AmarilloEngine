#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "../External/ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	//parent->AddComponent(this);
	paths = "No path";
	type = ComponentTypes::MESH;
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::SetMesh(ModuleMesh::Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	paths = path;
}

void ComponentMesh::OnEditor() {

	if (ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("aa");
		ImGui::Text("Index: %d", mesh->indices.size());
		ImGui::Text("Vertices: %d", mesh->ourVertex.size());
	}
}


