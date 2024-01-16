#include "Move.h"


Move2::Move2() : Amarillo()
{

}

void Move2::Start()
{
	gameObject->transform->SetLocalRotationEuler({ 0,0,90 });
}

void Move2::Update()
{
	Movement();
	Rotation();
}

void Move2::Movement()
{
	if (Input::GetKeyRepeat(SDL_SCANCODE_W))
	{
		velocity += acceleration;
	}
	if (Input::GetKeyRepeat(SDL_SCANCODE_S))
	{
		velocity -= acceleration;
	}
	velocity = Clamp(velocity, max_velocity_backward, max_velocity_forward);
	
	if (recoil > 0)
	{
		recoil -= friction_force;
		if (recoil <= 0)
		{
			recoil = 0;
		}
	}
	gameObject->transform->SetLocalPosition(gameObject->transform->GetPosition() * velocity - recoil_direction * recoil);

	if (velocity > 0)
	{
		velocity -= friction_force;
		if (velocity < 0)
		{
			velocity = 0;
		}
	}
	else if (velocity < 0)
	{
		velocity += friction_force;
		if (velocity > 0)
		{
			velocity = 0;
		}
	}
}

void Move2::Rotation()
{
	if (Input::GetKeyRepeat(SDL_SCANCODE_A))
	{
		angle += turning_velocity;
		gameObject->transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * DEGTORAD, 0));
	}

	if (Input::GetKeyRepeat(SDL_SCANCODE_D))
	{
		angle -= turning_velocity;
		gameObject->transform->SetLocalRotation(Quat::FromEulerXYZ(0, angle * DEGTORAD, 0));
	}	
}


float Move2::Clamp(const float& value, const float& min, const float& max)
{
	return (value > max) ? max : ((value < min) ? min : value);
}

AMARILLO_FACTORY Move2* CreateMove2() {
    Move2* move = new Move2();
    return move;
}