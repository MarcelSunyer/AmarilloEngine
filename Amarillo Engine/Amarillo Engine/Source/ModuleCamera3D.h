#pragma once
#include "Module.h"
#include "Globals.h"
#include "../External/MathGeoLib/include/Geometry/Frustum.h"
#include "..\External\MathGeoLib/include/Math/float3.h"
#include "..\External\MathGeoLib/include/Math/float4x4.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"
#include "..\External\ImGui/imgui_internal.h"

class Camera3D
{
public:
	Camera3D();

	void SetPosition(const float3& pos);
	const float3 GetPosition();

private:
	Frustum Camera_frustum;
	float	aspect_ratio = 0.0f;
	float   vertical_fov = 0.0f;

	bool	frustum_culling = true;
};
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