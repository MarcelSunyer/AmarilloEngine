#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "../External/Guid/uuid.h"


enum ResourceType
{
	RT_NULL,
	RT_TEXTURE,
	RT_MESH,
	RT_SCENE,
};

class Resource
{
public:
	Resource(uuids::uuid _unique_id, ResourceType type);
	virtual ~Resource();

	virtual void CleanUp();

	void SetFileName(const char* file_name);
	std::string GetFileName();

	ResourceType GetType();

	uuids::uuid GetUniqueId();

	bool IsUsed();
	int  UsedCount();

private:
	ResourceType type = RT_NULL;
	uuids::uuid  unique_id;
	int			 count_ref = 0;

	std::string  file_name;
};

#endif