//#pragma once
//
//#ifndef MODULE_GAMEOBJECT
//#define MODULE_GAMEOBJECT
//
//#include "Module.h"
//#include "Application.h"
//#include <string.h>
//#include "..\External\MathGeoLib/include/Math/float3.h"
//
//
//class GameObject : public Module
//{
//public:
//	GameObject(Application* app, const char name, const float3& translation, const float3& scale, const Quat& rotation);
//	~GameObject();
//	
//	void AddComponent();
//public:
//
//	GameObject* mParent;
//	//std::vector<Component*> mComponent;
//	std::vector<GameObject*> mChildren;
//	
//	
//
//	/*float3 GetLocalPosition() const;
//	float3 GetGlobalPosition() const;
//
//	float3 GetLocalRotation() const;
//	float3 GetLocalScale() const;
//
//	void SetLocalPosition(const float3& position);
//	void SetLocalRotation(const float3& XYZ_euler_rotation);
//	void SetLocalScale(const float3& scale);*/
//
//};
//
//class Component
//{
//public:
//	Component();
//	~Component();
//
//	virtual void OnEditor();
//
//	GameObject* mOwner;
//};
//
//class C_Transform : public Component
//{
//	void OnEditor() override;
//	float3 mPosition;
//	float3 mScale;
//	Quat* mRotation;
//
//	float4x4* mGlobalMatrix;
//	float4x4* mLocalMatrix;
//};
//#endif
