#include "ModuleGameObject.h"
#include "Application.h"

GameObject::GameObject(Application* app, const char name, const float3& translation, const float3& scale, const Quat& rotation) : Module(app)
{


}
GameObject::~GameObject()
{
	for (size_t i = 0; i < mComponent.size(); i++)
	{
		delete mComponent[i];
		mComponent[i] = nullptr;
	}
	for (size_t i = 0; i < mChildren.size(); i++)
	{
		delete mChildren[i];
		mChildren[i] = nullptr;
	}
}

void GameObject::AddComponent()
{



}
