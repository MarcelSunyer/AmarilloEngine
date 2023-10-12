#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

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
				App->editor->AddLog(("Application Init exits with ERROR"));
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				App->editor->AddLog("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				App->editor->AddLog("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input->quit == true)
			{
				state = MAIN_FINISH;
			}
		}
			break;

		case MAIN_FINISH:

			App->editor->AddLog("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				App->editor->AddLog("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	
	App->editor->AddLog(("Exiting game .\n", std::string( TITLE)));
	delete App;
	return main_return;
}