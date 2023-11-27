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
	editor_camera = new Camera3D();
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

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += editor_camera->Camera_frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= editor_camera->Camera_frustum.WorldRight() * speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= editor_camera->Camera_frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += editor_camera->Camera_frustum.front * speed;
		
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

			editor_camera->Camera_frustum.up = rotationQuat * editor_camera->Camera_frustum.up;

			editor_camera->Camera_frustum.front = rotationQuat * editor_camera->Camera_frustum.front;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(editor_camera->Camera_frustum.WorldRight(), DeltaY);

			editor_camera->Camera_frustum.up = rotationQuat * editor_camera->Camera_frustum.up;

			editor_camera->Camera_frustum.front = rotationQuat * editor_camera->Camera_frustum.front;

			if(editor_camera->Camera_frustum.up.y < 0.0f)
			{
				editor_camera->Camera_frustum.front = float3(0.0f, editor_camera->Camera_frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				editor_camera->Camera_frustum.up = editor_camera->Camera_frustum.front.Cross(editor_camera->Camera_frustum.WorldRight());
			}
		}
	}

	if (App->input->GetMouseZ() > 0)
	{
		newPos -= editor_camera->Camera_frustum.front * speed *3;
	}

	if (App->input->GetMouseZ() < 0)
	{
		newPos += editor_camera->Camera_frustum.front * speed*3;
	}

	editor_camera->Camera_frustum.pos -= newPos;

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		int mx = App->input->GetMouseXMotion();
		int my = App->input->GetMouseYMotion();

		float Sensitivity = 0.5f * dt;

		if (mx != 0)
		{
			float DeltaX = (float)mx * Sensitivity;
			newPos += -editor_camera->Camera_frustum.WorldRight() * DeltaX;
		}

		if (my != 0)
		{
			float DeltaY = (float)my * Sensitivity;
			newPos += editor_camera->Camera_frustum.up * DeltaY;
		}

		editor_camera->Camera_frustum.pos += newPos;

	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT && App->editor->GameObject_selected != nullptr)
	{
		LookAt(App->editor->GameObject_selected->transform->world_position);
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
	editor_camera->Camera_frustum.front = (Spot - editor_camera->Camera_frustum.pos).Normalized();
	float3 X = (float3(0.0f, 1.0f, 0.0f).Cross(editor_camera->Camera_frustum.front)).Normalized();
	editor_camera->Camera_frustum.up = editor_camera->Camera_frustum.front.Cross(X);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	editor_camera->Camera_frustum.pos += Movement;
	editor_camera->Camera_frustum.front += Movement;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	float4x4 tempMat4x4 = editor_camera->Camera_frustum.ViewMatrix();

	return tempMat4x4.Transposed().ptr();
}

float* ModuleCamera3D::GetProjectionMatrix()
{
	return editor_camera->Camera_frustum.ProjectionMatrix().Transposed().ptr();
}

void ModuleCamera3D::SetAspectRatio(int width, int height)
{
	float VerticalAspectRatio = (float)width / (float)height;

	editor_camera->Camera_frustum.verticalFov = 2.f * Atan(Tan(editor_camera->Camera_frustum.horizontalFov * 0.5f) / VerticalAspectRatio);
}

void ModuleCamera3D::SetAspectRatioGame(int width, int height)
{
	float VerticalAspectRatio = (float)width / (float)height;

	active_camera->Camera_frustum.verticalFov = 2.f * Atan(Tan(editor_camera->Camera_frustum.horizontalFov * 0.5f) / VerticalAspectRatio);
}

Camera3D* ModuleCamera3D::CreateCamera()
{

	Camera3D* ret = new Camera3D;
	cameras.push_back(ret);
	if (active_camera == nullptr)
	{
		active_camera = ret;

	}
	return ret;
}
void ModuleCamera3D::DestroyCamera(Camera3D* cam)
{
	/*for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end();)
	{
		if (cam == (*it))
		{
			RELEASE(*it);
			cameras.erase(it);
			break;
		}
		else
			++it;
	}*/
}

void ModuleCamera3D::DestroyAllCameras()
{
	/*for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end();)
	{
		RELEASE(*it);
		it = cameras.erase(it);
	}*/
}

std::vector<Camera3D*> ModuleCamera3D::GetCameras()
{
	return cameras;
}

Camera3D* ModuleCamera3D::GetEditorCamera()
{
	return editor_camera;
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

void Camera3D::SetPosition(const float3& pos)
{
	Camera_frustum.pos = pos;

}
const float3 Camera3D::GetPosition()
{
	return Camera_frustum.pos;
}

float3 Camera3D::GetZDir()
{
	return Camera_frustum.front;
}

float3 Camera3D::GetYDir()
{
	return Camera_frustum.up;
}

void Camera3D::GetCorners(float3* corners)
{
	Camera_frustum.GetCornerPoints(corners);
}

void Camera3D::SetNearPlaneDistance(const float& set)
{
	Camera_frustum.nearPlaneDistance = set;
}

void Camera3D::SetFarPlaneDistance(const float& set)
{
	Camera_frustum.farPlaneDistance = set;
}

void Camera3D::SetFOV(const float& set)
{
	Camera_frustum.verticalFov = set;
}

void Camera3D::SetAspectRatio()
{
	Camera_frustum.AspectRatio();
}

const float Camera3D::GetNearPlaneDistance() const
{
	return Camera_frustum.nearPlaneDistance;
}

const float Camera3D::GetFarPlaneDistance() const
{
	return Camera_frustum.farPlaneDistance;
}

const float Camera3D::GetVerticalFOV() const
{
	return Camera_frustum.verticalFov;
}

const float Camera3D::GetHorizontalFOV() const
{
	return Camera_frustum.horizontalFov;
}

const float4x4 Camera3D::GetViewMatrix() const
{
	return Camera_frustum.ViewMatrix();
}

const float4x4 Camera3D::GetProjectionMatrix() const
{
	return Camera_frustum.ProjectionMatrix();
}

const float4x4 Camera3D::GetOpenGLViewMatrix() const
{
	float4x4 view = Camera_frustum.ViewMatrix();
	return view.Transposed();
}

const float4x4 Camera3D::GetOpenGLProjectionMatrix() const
{
	return Camera_frustum.ProjectionMatrix().Transposed();
}

void Camera3D::MoveFront(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += Camera_frustum.front * speed;
	Camera_frustum.Translate(movement);
}
void Camera3D::MoveBack(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= Camera_frustum.front * speed;
	Camera_frustum.Translate(movement);
}

void Camera3D::MoveRight(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += Camera_frustum.WorldRight() * speed;
	Camera_frustum.Translate(movement);
}

void Camera3D::MoveLeft(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= Camera_frustum.WorldRight() * speed;
	Camera_frustum.Translate(movement);
}

void Camera3D::MoveUp(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	Camera_frustum.Translate(movement);
}

void Camera3D::MoveDown(const float& speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	Camera_frustum.Translate(movement);
}

Frustum Camera3D::GetFrustum()
{
	return Camera_frustum;
}

void Camera3D::Orbit(const float3& rotate_center, const float& motion_x, const float& motion_y)
{
	float3 distance = Camera_frustum.pos - rotate_center;

	Quat X(Camera_frustum.WorldRight(), motion_y);
	Quat Y(Camera_frustum.up, motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	Camera_frustum.pos = distance + rotate_center;
}

void Camera3D::Rotate(const float& motion_x, const float& motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	Camera_frustum.front = rotation_x.Mul(Camera_frustum.front).Normalized();
	Camera_frustum.up = rotation_x.Mul(Camera_frustum.up).Normalized();

	Quat rotation_y = Quat::RotateAxisAngle(Camera_frustum.WorldRight(), motion_y);
	Camera_frustum.front = rotation_y.Mul(Camera_frustum.front).Normalized();
	Camera_frustum.up = rotation_y.Mul(Camera_frustum.up).Normalized();
}

void Camera3D::Look(const float3& look_pos)
{
	float3 dir = look_pos - Camera_frustum.pos;

	float3x3 direction_matrix = float3x3::LookAt(Camera_frustum.front, dir.Normalized(), Camera_frustum.up, float3::unitY);

	Camera_frustum.front = direction_matrix.MulDir(Camera_frustum.front).Normalized();
	Camera_frustum.up = direction_matrix.MulDir(Camera_frustum.up).Normalized();
}
void Camera3D::Focus(const float3& focus_center, const float& distance)
{
	float3 dir = Camera_frustum.pos - focus_center;
	Camera_frustum.pos = dir.Normalized() * distance;

	Look(focus_center);
}

void Camera3D::Focus(const AABB& aabb)
{
	Focus(aabb.CenterPoint(), aabb.Size().Length());
}
//Meter esto en una funcion para poder meter difernetes camaras en la funcion de arriva
float* Camera3D::GetViewMatrix()
{
	float4x4 tempMat4x4 = Camera_frustum.ViewMatrix();

	return tempMat4x4.Transposed().ptr();
}

float* Camera3D::GetProjectionMatrix()
{
	return Camera_frustum.ProjectionMatrix().Transposed().ptr();
}

void Camera3D::OnClick(float pos_x, float pos_y)
{
	float2 size = applic->window->GetWindowSize();
	float normalPos_x = pos_x / size[0];
	float normalPos_y = pos_y / size[1];

	normalPos_x = (normalPos_x - 0.5) / 0.5;
	normalPos_y = (normalPos_y - 0.5) / 0.5;

	latest_ray = Camera_frustum.UnProjectLineSegment(normalPos_x, normalPos_y);

	applic->scene->TestGameObjectSelection(latest_ray);
}
