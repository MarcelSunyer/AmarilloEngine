#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"

#include "..\External\Assimp/include/cimport.h"
#include "..\External\Assimp/include/scene.h"
#include "..\External\Assimp/include/postprocess.h"

#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

#include "..\External\SDL/include/SDL.h"

#pragma comment( lib, "External/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "External/SDL/libx86/SDL2main.lib" )



enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG(("Application Init exits with ERROR"));
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input->quit == true)
			{
				state = MAIN_FINISH;
			}
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}
	
	LOG(("Exiting game %S.\n", TITLE));
	applic = nullptr;
	delete App;
	return main_return;
}