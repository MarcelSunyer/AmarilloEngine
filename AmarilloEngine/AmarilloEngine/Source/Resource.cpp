#include "Resource.h"

Resource::Resource(uuids::uuid _unique_id, ResourceType _type)
{
	unique_id = _unique_id;
	type = _type;
	count_ref = 0;
}
Resource::~Resource()
{
}

void Resource::CleanUp()
{
}

void Resource::SetFileName(const char* _file_name)
{
	file_name = _file_name;
}

std::string Resource::GetFileName()
{
	return file_name;
}

ResourceType Resource::GetType()
{
	return type;
}

uuids::uuid Resource::GetUniqueId()
{
	return unique_id;
}

bool Resource::IsUsed()
{
	return count_ref > 0 ? true : false;
}

int Resource::UsedCount()
{
	return count_ref;
}