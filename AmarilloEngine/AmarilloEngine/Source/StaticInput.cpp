#include "StaticInput.h"
#include "Application.h"
#include "ModuleInput.h"

bool Input::GetKeyDown(const SDL_Scancode& code)
{
    return applic->input->GetKey(code) == KEY_DOWN;
}

bool Input::GetKeyIdle(const SDL_Scancode& code)
{
    return applic->input->GetKey(code) == KEY_IDLE;
}

bool Input::GetKeyUp(const SDL_Scancode& code)
{
    return applic->input->GetKey(code) == KEY_UP;
}

bool Input::GetKeyRepeat(const SDL_Scancode& code)
{
    return applic->input->GetKey(code) == KEY_REPEAT;
}


float3 Input::GetMousePosition()
{
    float3 Mouse_Pos = { (float)applic->input->GetMouseX(),(float)applic->input->GetMouseY() ,(float)applic->input->GetMouseZ() };
    return Mouse_Pos;

}

int Input::GetMouseY()
{
    return applic->input->GetMouseY();

}

int Input::GetMouseX()
{
    return applic->input->GetMouseX();

}

float Input::GetMouseWheel()
{
    return applic->input->GetMouseZ();
}

float Input::GetMouseYMotion()
{
    return applic->input->GetMouseYMotion();
}

float Input::GetMouseXMotion()
{
    return applic->input->GetMouseXMotion();
}

bool Input::GetMouseButtonDown(const MOUSE_BUTTONS& button)
{
    return applic->input->GetMouseButton(button) == KEY_DOWN;
}

bool Input::GetMouseButtonUp(const MOUSE_BUTTONS& button)
{
    return applic->input->GetMouseButton(button) == KEY_UP;
}

bool Input::GetMouseButtonIdle(const MOUSE_BUTTONS& button)
{
    return applic->input->GetMouseButton(button) == KEY_IDLE;
}

bool Input::GetMouseButtonRepeat(const MOUSE_BUTTONS& button)
{
    return applic->input->GetMouseButton(button) == KEY_REPEAT;
}