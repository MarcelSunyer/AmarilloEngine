#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"
#include "../External/ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::MESH;
}


void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::SetMesh(ModuleMesh::Mesh* mesh)
{
	this->mesh = mesh;
	InitBoundingBoxes();
}

void ComponentMesh::SetPath(std::string path)
{
	paths = path;
}

void ComponentMesh::OnEditor() {

	if (ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Path: %s", paths),
		ImGui::Text("Index: %d", mesh->indices.size());
		ImGui::Text("Vertices: %d", mesh->ourVertex.size());
	}
}

void ComponentMesh::DebugDraw()
{
	UpdateBoundingBoxes();
	RenderBoundingBoxes();
}

void ComponentMesh::InitBoundingBoxes()
{
	obb.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	if (mesh != NULL)
	{
		std::vector<float3> floatArray;
		floatArray.reserve(mesh->ourVertex.size());

		for (const auto& vertex : mesh->ourVertex) {
			floatArray.push_back(vertex.Position);
		}

		aabb.SetFrom(&floatArray[0], floatArray.size());
	}
}

void ComponentMesh::UpdateBoundingBoxes()
{
	if (mesh != nullptr)
	{
		obb = aabb;
		obb.Transform(owner->transform->world_matrix);

		globalAABB.SetNegativeInfinity();
		globalAABB.Enclose(obb);

	}
	else
	{
		LOG("Error: GameObject or its transform is null");
	}
}

void ComponentMesh::RenderBoundingBoxes()
{
	float3 verticesO[8];
	obb.GetCornerPoints(verticesO);
	applic->renderer3D->DrawBoundingBox(verticesO, float3(0, 255, 0));

	float3 verticesA[8];
	globalAABB.GetCornerPoints(verticesA);
	applic->renderer3D->DrawBoundingBox(verticesA, float3(0, 255, 0));

}







