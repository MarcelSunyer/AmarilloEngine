#include "Tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{

}

void Tank::Start(GameObject* tank)
{
	game_object = tank;

	GameObject* wheels = GameObject::FindWithName("Lower_Tank");
	if (wheels != nullptr)
	{
		wheels_transform = (ComponentTransform*)wheels->GetComponent(ComponentTypes::TRANSFORM);
	}

	GameObject* turret = GameObject::FindWithName("TankTurret");
	if (turret != nullptr)
	{
		turret_transform = (ComponentTransform*)turret->GetComponent(ComponentTypes::TRANSFORM);
	}
}

void Tank::Update()
{
	Movement();
	Rotation();
	Shoot();
}

void Tank::Shoot()
{
	// Shooting - TODO
	/*if (Input::GetMouseButtonDown(Input::MOUSE_LEFT_BUTTON))
	{
		GameObject* bullet_created = bullet.ConvertToGameObject((float3{ transform->GetGlobalPosition().x,transform->GetGlobalPosition().y + 1.5f,transform->GetGlobalPosition().z }) + turret_transform->forward * 1.2f);

		if (bullet_created != nullptr)
		{
			Bullet* minion = (Bullet*)bullet_created->GetComponentScript("Bullet");
			minion->bullet_direction = turret_transform->forward;
			recoil_direction = turret_transform->forward;
			recoil = recoil_velocity;
		}
	}*/
}

void Tank::Movement() {

	if (applic->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity += acceleration;
		LOG("Mov. W")
	}
	if (applic->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity -= acceleration;
		LOG("Mov. S")
	}

	velocity = Clamp(velocity, max_velocity_backward, max_velocity_forward);

	if (recoil > 0) {
		recoil -= friction_force;
		if (recoil <= 0) {
			recoil = 0;
		}
	}

	game_object->transform->SetLocalPosition(game_object->transform->GetPosition() + wheels_transform->GetForwardVector() * velocity - recoil_direction * recoil);

	if (velocity > 0) {
		velocity -= friction_force;
		if (velocity < 0) {
			velocity = 0;
		}
	}
	else if (velocity < 0) {
		velocity += friction_force;
		if (velocity > 0) {
			velocity = 0;
		}
	}
}

void Tank::Rotation()
{
	if (applic->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		angle += turning_velocity;
		wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * DEGTORAD, 0));
	}

	if (applic->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		angle -= turning_velocity;
		wheels_transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * DEGTORAD, 0));
	}

	float2 mouse = { (float)applic->input->GetMouseX(), (float)applic->input->GetMouseY() };
	float3 rotation = turret_transform->GetRotation().ToEulerXYZ();
	int width = applic->window->GetWindowSize().x;
	int middle = width / 2;
	int height = applic->window->GetWindowSize().y;

	float3 wheels = wheels_transform->GetRotation().ToEulerXYZ() * RADTODEG;

	if (wheels.x == 0 && wheels.y > 0)
	{
		rotation.y = (-(mouse.x * 180 / width) + 90) + (wheels.y);

		if (mouse.y > (height / 2))
		{
			rotation.x = -180;
			rotation.z = -180;
			rotation.y -= wheels.y * 2;
		}
		else
		{
			rotation.x = 0;
			rotation.z = 0;
		}
	}

	else if (wheels.x == 0 && wheels.y < 0)
	{
		rotation.y = ((mouse.x * 180 / width)) - (wheels.y);

		if (mouse.y > (height / 2))
		{
			rotation.y -= (-90 - wheels.y * 2);
			rotation.x = 0;
			rotation.z = 0;
		}
		else
		{
			rotation.y -= 270;
			rotation.x = -180;
			rotation.z = -180;
		}
	}

	else if ((wheels.x == -180 || wheels.x == 180) && wheels.y > 0)
	{
		rotation.y = (-(mouse.x * 180 / width)) - (wheels.y);

		if (mouse.y > (height / 2))
		{
			rotation.y -= (90 - wheels.y * 2);
			rotation.x = -180;
			rotation.z = -180;
		}
		else
		{
			rotation.y -= 90;
			rotation.x = 0;
			rotation.z = 0;
		}
	}

	else if ((wheels.x == -180 || wheels.x == 180) && wheels.y < 0)
	{
		rotation.y = (-(mouse.x * 180 / width));

		if (mouse.y > (height / 2))
		{
			rotation.x = -180;
			rotation.z = -180;
			rotation.y += wheels.y - 90;
		}
		else
		{
			rotation.x = 0;
			rotation.z = 0;
			rotation.y -= wheels.y + 90;
		}
	}

	turret_transform->SetLocalRotation(Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD));
}
