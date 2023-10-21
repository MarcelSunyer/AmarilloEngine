#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL\include\SDL_opengl.h"
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SDL/include/SDL_syswm.h"
#include "Primitive.h"


// ImGui includes
//
//#include "ImGui/imgui.h"
//#include "ImGui/backends/imgui_impl_sdl2.h"
//#include "ImGui/backends/imgui_impl_opengl3.h"



#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/MGLDebug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/MGLRelease/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG

static const GLfloat CubeVertices[] = {
	-1, -1, -1,
	1, -1, -1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, 1, 1,
	-1, 1, 1
};
static const GLuint CubeIndices[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled), context()
{
	myModel = new LoadFBX(app);
	Juan = new Primitive(app);
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	delete(myModel);
	delete(Juan);
}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	App->editor->AddLog("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		App->editor->AddLog("OpenGL context could not be created! SDL_Error: %s\n" + std::string(SDL_GetError()));
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->editor->AddLog("Warning: Unable to set VSync! SDL Error: \n" + std::string(SDL_GetError()));

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		const char *errorMessage = reinterpret_cast<const char*>(error);
		
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog("Error initializing OpenGL! \n" + std::string(errorMessage));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog(("Error initializing OpenGL! \n" + std::string(errorMessage)));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			App->editor->AddLog(("Error initializing OpenGL! \n" + std::string(errorMessage)));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	//Init Glew
	glewInit();

	//Init Devil
	//InitDevil();

	//myModel.Load("../BakerHouse.fbx");

	VBO = 0;
	EBO = 0;
	VAO = 0;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//Pruebas LoadTexture
	App->renderer3D->myModel->error = false;
	App->renderer3D->myModel->isLoaded = false;
	// Esto es un path fijo, no se como hacerlo relativo
	//myModel_path = "C:/Users/cacah/OneDrive/Documentos/GitHub/Game_Engine/Amarillo Engine/Amarillo Engine/Assets/BakerHouse.fbx";
	//myModel_texture_path = "C:/Users/cacah/OneDrive/Documentos/GitHub/Game_Engine/Amarillo Engine/Amarillo Engine/Assets/Baker_house.png";
	//myModel_texture_path = "C:/Users/cacah/OneDrive/Documentos/GitHub/Game_Engine/Amarillo Engine/Amarillo Engine/Assets/green.png";

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
	if (myModel_path != nullptr && !myModel->isLoaded && myModel->error == false)
	{
		
		myModel->Load(myModel_path);
		myModel->TextureImport(myModel_texture_path);
		if (myModel->error == false)
		{
			App->editor->AddLog("Loaded MyModelPath");
		}
		else
		{
			App->editor->AddLog("An error ocurred loading the model (FBX mode not compatible)");
		}
		
	}
	
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	Grid.Render();
	if (activeWire)
	{
		
		Juan->wire |= true;
		Juan->Render();
	}
	if (!activeWire)
	{
		
		Juan->wire = false;
		Juan->Render();
	}

	myModel->Draw();

	App->editor->DrawEditor();

	glBindVertexArray(VAO);

	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set drawing color (red) // x, y, width, height

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	App->editor->AddLog(("Destroying 3D Renderer"));

	SDL_GL_DeleteContext(context);

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::InitDevil()
{
	ilInit();		//No se porque pero este peta
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	App->console->AddLog("Devil has initialised successfully");
}
