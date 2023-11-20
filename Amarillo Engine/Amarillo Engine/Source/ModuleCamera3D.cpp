#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "Primitive.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "..\External\MathGeoLib\include\Math\Quat.h"
#include "..\External\ImGui/imgui.h"
#include "../External/MathGeoLib/include/Geometry/Frustum.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0,3,-10);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 1000.0f;

	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG(("Setting up the camera"));
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG(("Cleaning camera"));

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0, 0, 0);
	float speed = 5.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;


	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= frustum.WorldRight() * speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += frustum.front * speed;
		
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;


		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.35f * dt;


		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			frustum.up = rotationQuat * frustum.up;

			frustum.front = rotationQuat * frustum.front;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(frustum.WorldRight(), DeltaY);

			frustum.up = rotationQuat * frustum.up;

			frustum.front = rotationQuat * frustum.front;

			if(frustum.up.y < 0.0f)
			{
				frustum.front = float3(0.0f, frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				frustum.up = frustum.front.Cross(frustum.WorldRight());
			}
		}
	}

	if (App->input->GetMouseZ() > 0)
	{
		newPos -= frustum.front * speed *3;
	}

	if (App->input->GetMouseZ() < 0)
	{
		newPos += frustum.front * speed*3;
	}

	frustum.pos -= newPos;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int mx = App->input->GetMouseXMotion();
		int my = App->input->GetMouseYMotion();

		float Sensitivity = 0.5f * dt;

		if (mx != 0)
		{
			float DeltaX = (float)mx * Sensitivity;
			newPos += -frustum.WorldRight() * DeltaX;
		}

		if (my != 0)
		{
			float DeltaY = (float)my * Sensitivity;
			newPos += frustum.up * DeltaY;
		}

		frustum.pos += newPos;

	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT && App->editor->GameObject_selected != nullptr)
	{
		LookAt(App->editor->GameObject_selected->transform->new_position);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		LookAt(float3(0,0,0));
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{	
	frustum.front = (Spot - frustum.pos).Normalized();
	float3 X = (float3(0.0f, 1.0f, 0.0f).Cross(frustum.front)).Normalized();
	frustum.up = frustum.front.Cross(X);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	frustum.pos += Movement;
	frustum.front += Movement;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	float4x4 tempMat4x4 = frustum.ViewMatrix();

	return tempMat4x4.Transposed().ptr();
}

Camera3D::Camera3D()
{
	Camera_frustum.type = FrustumType::PerspectiveFrustum;
	Camera_frustum.pos = float3(0, 3, -10);
	Camera_frustum.front = float3::unitZ;
	Camera_frustum.up = float3::unitY;

	Camera_frustum.nearPlaneDistance = 1.0f;
	Camera_frustum.farPlaneDistance = 1000.0f;

	Camera_frustum.verticalFov = 60.0f * DEGTORAD;
	Camera_frustum.horizontalFov = 2.0f * atanf(tanf(Camera_frustum.verticalFov / 2.0f) * 1.3f);
}
