#ifndef __RESOURCE_PREFAB_H__
#define __RESOURCE_PREFAB_H__

#include "Resource.h"
#include <string>

class GameObject;

class ResourcePrefab : public Resource
{
public:
	ResourcePrefab(uuids::uuid unique_id);
	virtual ~ResourcePrefab();

	void CleanUp();

	void SetGameObject(GameObject* gameobject);
	void Instantiate(GameObject* parent = nullptr);
	void SerializeCustom(const char* path, const char* name, const char* extension);

};

#endif //__RESOURCE_PREFAB_H__