#ifndef _TANK_H_
#define _TANK_H_

#include "../../AmarilloEngine/Source/Amarillo.h"
#include "AmarilloScripts.h"

class GameObject;
class ComponentTransform;

class AMARILLO_ENGINE_API Tank : Amarillo {

public:

	Tank();
	virtual ~Tank();

	void Start(GameObject* tank);
	void Update();

	void Shoot();
	void Movement();
	void Rotation();

public:

	ComponentTransform* wheels_transform = nullptr;
	ComponentTransform* turret_transform = nullptr;
	GameObject* game_object;

	float max_velocity_forward = 0.1f;
	float max_velocity_backward = -0.1f;

	float velocity = 0.0f;
	float friction_force = 0.05f;
	float acceleration = 2.0f;
	float recoil = 0.0f;
	float recoil_velocity = 6.0f;
	float3 recoil_direction = { 0,0,0 };
	float turning_velocity = 3.0f;

private:

	float angle = 0.0f;
	float3 to_look = { 0,0,0 };
};
#endif // !_TANK_H_