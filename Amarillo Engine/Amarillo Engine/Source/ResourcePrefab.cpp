#include "ResourcePrefab.h"

ResourcePrefab::ResourcePrefab(uuids::uuid unique_id) : Resource(unique_id, ResourceType::RT_PREFAB)
{

}

ResourcePrefab::~ResourcePrefab()
{
}

void ResourcePrefab::CleanUp()
{
}

void ResourcePrefab::SetGameObject(GameObject* go)
{
}

void ResourcePrefab::Instantiate(GameObject* parent)
{
}

void ResourcePrefab::SerializeCustom(const char* path, const char* name, const char* extension)
{
}
