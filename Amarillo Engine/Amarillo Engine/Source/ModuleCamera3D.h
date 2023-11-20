#pragma once
#include "Module.h"
#include "Globals.h"
#include "../External/MathGeoLib/include/Geometry/Frustum.h"
#include "..\External\MathGeoLib/include/Math/float3.h"
#include "..\External\MathGeoLib/include/Math/float4x4.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"
#include "..\External\ImGui/imgui_internal.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	float* GetViewMatrix();

public:

	bool holdingWindowTab = false;
	 // Use ImGui function to get tab height

	bool windowMovement;
	

public:
 
	Frustum frustum; 
};