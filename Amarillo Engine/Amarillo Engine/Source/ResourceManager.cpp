#include "ResourceManager.h"
#include "../External/ImGui/imgui.h"

ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Init()
{
	return false;
}

bool ModuleResourceManager::Start()
{
	return false;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	return update_status();
}

bool ModuleResourceManager::CleanUp()
{
	return false;
}

void ModuleResourceManager::GetUID()
{
    ImGui::Begin("Resource Manager");

    // Mostrar información sobre los recursos cargados
    ImGui::Text("Total Resources: %i", resources.size());
    for (auto it = resources.begin(); it != resources.end(); ++it) {
        ImGui::Text("UID: %i, Ref Count: %i, Type: %i",
            (*it).second->GetUID(),
            (*it).second->GetReferenceCount(),
            static_cast<int>((*it).second->GetType())
        );
    }

    // Otros elementos de la interfaz de usuario según sea necesario

    ImGui::End();
}
