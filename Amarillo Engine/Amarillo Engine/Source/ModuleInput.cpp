#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "..\External\ImGui\backends/imgui_impl_sdl2.h"
#include "ModuleEditor.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG(("Init SDL input event system"));
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG(("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError()));
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
			}
			case SDL_DROPFILE:
			{
				if (e.drop.file != ERROR)
				{
					// e.drop.file has the directoy of the droped file
					char* dropped_filedir = e.drop.file;
					const char* ext = strrchr(dropped_filedir, '.');

					if (ext != NULL)
					{
						if (strcmp(ext, ".fbx") == 0 || strcmp(ext, ".FBX") == 0 )
						{
							LOG(("Mesh dropped on window - File Path: %s", dropped_filedir));
							// Load mesh
							App->mesh->LoadMesh(dropped_filedir);
							App->renderer3D->BindBuffers();
						}
						else if ((strcmp(ext, ".png") == 0 || strcmp(ext, ".dds") == 0) && App->editor->GameObject_selected != nullptr)
						{
							LOG(("Texture dropped on window - File Path: %s", dropped_filedir));
							

							//1. Comprobar si hay textura / es nula
							if (App->editor->GameObject_selected->GetComponent(ComponentTypes::TEXTURE) != nullptr)
							{
								//Has texture - Swap the textures

								dynamic_cast<ComponentTexture*>(App->editor->GameObject_selected->GetComponent(ComponentTypes::TEXTURE))->SwapTextures(dropped_filedir);

							}
							else
							{
								//No texture

								// Load texture							
								App->texture->LoadTexture(dropped_filedir);
							}

							
							
							App->renderer3D->BindBuffers();
						}
					}

					// Free memory
					SDL_free(dropped_filedir);

					
				}
				break;

			}
		}
		

	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG(("Quitting SDL input event subsystem"));
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}