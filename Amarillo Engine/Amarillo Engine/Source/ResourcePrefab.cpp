#include "ResourcePrefab.h"


ResourcePrefab::ResourcePrefab(uuids::uuid unique_id, JSON_Doc* json) : Resource(unique_id, ResourceType::RT_PREFAB)
{
	prefab_json = json;
}

ResourcePrefab::~ResourcePrefab()
{
}

void ResourcePrefab::CleanUp()
{

}


