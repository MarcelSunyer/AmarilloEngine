//TODO: Write a script that do things, like moves a GameObject
#ifndef _MOVE_H_
#define _MOVE_H_

#include "../../AmarilloEngine/Source/Amarillo.h"
#include "AmarilloScripts.h"



class AMARILLO_ENGINE_API Move2 : Amarillo{
public:
	Move2();
	
	void Start() override;

	void Update() override;

	void Movement();

	void Rotation();

	static float Clamp(const float& value, const float& min, const float& max);
public: 
	float max_velocity_forward = 8.0f;
	float max_velocity_backward = -8.0f;

	float velocity = 0.0f;
	float friction_force = 0.05f;
	float acceleration = 4.0f;
	float recoil = 0.0f;
	float recoil_velocity = 6.0f;
	float3 recoil_direction = { 0,0,0 };
	float turning_velocity = 30.0f;

	float angle = 0.0f;

};
AMARILLO_FACTORY Move2* CreateMove2();

#endif // !_MOVE_H_
