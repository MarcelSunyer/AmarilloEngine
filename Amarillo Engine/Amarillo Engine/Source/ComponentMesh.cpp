#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "../External/ImGUI/imgui.h"
#include "LoadFBX.h"

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
