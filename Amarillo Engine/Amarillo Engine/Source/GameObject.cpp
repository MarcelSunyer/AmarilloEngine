#include "GameObject.h"


GameObject::GameObject(const char name, float3 transform, float3 scale, Quat* rot)
{
	mName = name;

}

GameObject::~GameObject()
{
	/*for (size_t i = 0; i < mComponents.size(); i++)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}*/

	for (size_t i = 0; i < mChildren.size(); i++)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}
}
//void GameObject::AddComponent(ComponentTypes type)
//{
//	//Component* ret = nullptr;
//
//	//switch (type)
//	//{
//	//	//Meter los componentes de cada tipo
//	//case ComponentTypes::GEOMETRY:
//	//	//ret = new ComponentGeometry(this);
//	//	break;
//	//case ComponentTypes::MATERIAL:
//	//	//ret = new ComponentMaterial(this);
//	//	break;
//	//}
//
//	//if (ret != nullptr)
//	//	mComponents.push_back(ret);
//	return;
//}

float3 GameObject::GetLocalPosition()
{
	return translation;
}

float3 GameObject::GetWorldPosition()
{
	return float3::zero;
}

float3 GameObject::GetLocalScale()
{
	return scale;
}

Quat* GameObject::GetLocalRotation()
{
	//return rotation.ToEulerXYZ();
	return 0;
}



