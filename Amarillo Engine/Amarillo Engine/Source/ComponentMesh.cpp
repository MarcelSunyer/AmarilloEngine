#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "../External/ImGUI/imgui.h"
#include "LoadFBX.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	//parent->AddComponent(this);
	paths = "No path";
	mesh = nullptr;
	type = ComponentTypes::MESH;
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::SetMesh(aiMesh* mesh)
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
		
		if (mesh != NULL) {
			ImGui::Text("aa");
			ImGui::Text("Index: %d", mesh->mNumFaces);
			ImGui::Text("Vertices: %d", mesh->mNumVertices);
		}
		//Todo:Falta poner estas dos

		/*ImGui::Text("Normals: %d", mesh->);
		ImGui::Text("texture: %d", mesh->);*/
	}
}


