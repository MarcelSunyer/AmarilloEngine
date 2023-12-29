#ifndef __RESOURCE_PREFAB_H__
#define __RESOURCE_PREFAB_H__

#include "Resource.h"
#include <string>
#include "ModuleJSON.h"

class GameObject;

class ResourcePrefab : public Resource
{
public:
	ResourcePrefab(uuids::uuid unique_id, JSON_Doc* json);
	virtual ~ResourcePrefab();

	void CleanUp();

private:
	JSON_Doc* prefab_json;
};

#endif //__RESOURCE_PREFAB_H__