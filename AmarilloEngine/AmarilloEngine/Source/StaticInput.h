#pragma once
#include "../External/SDL/include/SDL_scancode.h"
#include "../External/MathGeoLib/include/Math/float3.h"

enum KEY_STATE;

extern "C" __declspec(dllexport) class Input {
public:

	enum {
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_REPEAT,
		KEY_UP
	};

	enum MOUSE_BUTTONS {
		MOUSE_LEFT_BUTTON = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_RIGHT_BUTTON = 3,
	};


	static bool GetKeyDown(const SDL_Scancode& code);
	static bool GetKeyIdle(const SDL_Scancode& code);
	static bool GetKeyUp(const SDL_Scancode& code);
	static bool GetKeyRepeat(const SDL_Scancode& code);



	static float3 GetMousePosition();
	static int GetMouseY();
	static int GetMouseX();

	static float GetMouseWheel();
	static float GetMouseYMotion();
	static float GetMouseXMotion();
	static bool GetMouseButtonDown(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonUp(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonIdle(const MOUSE_BUTTONS& button);
	static bool GetMouseButtonRepeat(const MOUSE_BUTTONS& button);
};
