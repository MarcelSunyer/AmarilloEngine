#pragma once

#include "Globals.h"
#include "Application.h"

#include"GameObject.h"
#include"ResourceMesh.h"

#include "ComponentMesh.h"
#include "ComponentScript.h"
#include "ComponentTransform.h"
		 
#include "ModuleInput.h"
#include "ModuleScene.h"

		 
#include "GameObject.h"
#include "../External/MathGeoLib/include/Math/float3.h"


float GetGamepadRightTrigger()
{
	/*if (External != nullptr)
		return External->input->GetGamepadRightTriggerValue();
		*/
	return 0;
}

//void PlayHaptic(float strength, int length) {
//	if (External != nullptr) {
//		External->input->PlayHaptic(strength, length);
//	}
//}
//
//int CS_GetControllerType()
//{
//	if (External != nullptr)
//		return External->input->GetControllerType();
//
//	return 0;
//}