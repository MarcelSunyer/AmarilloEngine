#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "..\External\SDL\include\SDL_opengl.h"
#include "..\External\SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "..\External\SDL/include/SDL_syswm.h"
#include "Primitive.h"
#include "ModuleMesh.h"




// ImGui includes
//
//#include "ImGui/imgui.h"
//#include "ImGui/backends/imgui_impl_sdl2.h"
//#include "ImGui/backends/imgui_impl_opengl3.h"



#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "External/Glew/libx86/glew32.lib")


#ifdef _DEBUG
#pragma comment (lib, "External/MathGeoLib/libx86/MGLDebug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "External/MathGeoLib/libx86/MGLRelease/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
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
	
	PrimitiveTest = new Primitive(app);
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	
	delete(PrimitiveTest);
}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n",SDL_GetError);
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s \n",SDL_GetError);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		const char *errorMessage = reinterpret_cast<const char*>(error);
		
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", errorMessage);
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s \n", errorMessage);
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
			LOG("Error initializing OpenGL! %s \n", errorMessage);
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(5.0f, 5.0f, 5.0f);
		lights[0].Init();

		lights[1].ref = GL_LIGHT1;
		lights[1].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[1].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[1].SetPos(-5.0f, -5.0f, -5.0f);
		lights[1].Init();

		
		
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		lights[1].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		//Init Glew
		glewInit();

		//Init Devil
		InitDevil();
	}
	//Profe
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	for (int i = 0; i < CHECKERS_WIDTH; i++) {
		for (int j = 0; j < CHECKERS_HEIGHT; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkTexture);
	glBindTexture(GL_TEXTURE_2D, checkTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	BindBuffers();

	Grid.axis = true;

	ilInit();


	App->scene->LoadMeshAndTexture("../Assets/Skybox.fbx", "../Assets/skybox.png")->mName = "Skybox";
	App->scene->LoadMeshAndTexture("../Assets/BakerHouse.fbx", "../Assets/Baker_house.png")->mName = "Baker House";


	BindBuffers();
	CreateMainBuffer();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	RenderMainBuffer(true);
	Grid.Render();

	App->mesh->UpdateBoundingBoxes(App->scene->game_objects);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->GetViewMatrix());
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//myModel->Draw();

	if (activeWire)
	{
		
		PrimitiveTest->wire |= true;
		PrimitiveTest->Render();
	}
	if (!activeWire)
	{
		
		PrimitiveTest->wire = false;
		PrimitiveTest->Render();
	}
	
	
	std::vector<GameObject*> gameObject_list = App->scene->GetGameObjects();

	for (uint n = 0; n < gameObject_list.size(); n++)
	{

		GameObject* gameobject = gameObject_list[n];

		for (uint m = 0; m < gameobject->components.size(); m++)
		{
			//Pregunta profe al borrar el 
			Component* component = gameobject->components[m];
			if (component->type != ComponentTypes::MESH)
			{
				continue;
			}

			ComponentMesh* componentMesh = (ComponentMesh*)component;

			float4x4 matrix = float4x4::FromTRS(float3(5,1,1),Quat::identity, float3(1,1,1));

			glPushMatrix();
			glMultMatrixf(gameobject->transform->GetTransformMatrix().Transposed().ptr());
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_COORD_ARRAY);
			//Bind Mesh
			glBindBuffer(GL_ARRAY_BUFFER, componentMesh->mesh->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, componentMesh->mesh->EBO);
			glEnableClientState(GL_VERTEX_ARRAY);	
			glEnableClientState(GL_NORMAL_ARRAY);	
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)0);

			//Bind Textures
			if (gameobject->GetComponent(ComponentTypes::TEXTURE) != nullptr)
			{
				const Texture* mTexture = dynamic_cast<ComponentTexture*>(gameobject->GetComponent(ComponentTypes::TEXTURE))->GetTexture();

				if (mTexture != nullptr)
				{


					glBindTexture(GL_TEXTURE_2D, mTexture->textID);
				}
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, checkTexture);
			}

			glNormalPointer(GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)offsetof(ModuleMesh::Vertex, Normal));
			glTexCoordPointer(2, GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)offsetof(ModuleMesh::Vertex, TexCoords));

			glDrawElements(GL_TRIANGLES, componentMesh->mesh->indices.size(), GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_COORD_ARRAY);
			glPopMatrix();
		}

	}
	
	
	Grid.Render();

	if (activeNormals)
	{
		App->mesh->DrawNormals();
	}
	else
	{

	}
	RenderMainBuffer(false);
	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < App->mesh->ourMeshes.size(); i++) {

		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->VBO);
		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->EBO);
	}
	DeleteMainBuffer();

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);

	glLoadMatrixf(App->camera->frustum.ProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::InitDevil()
{
	ilInit();		
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	LOG("Devil has initialised successfully");
	App->console->AddLog("Devil has initialised successfully");
}

void ModuleRenderer3D::DrawBoundingBox(float3* vertices, float3 color)
{

	uint indices[24] = {

		0,2,2,
		6,6,4,
		4,0,0,
		1,1,3,
		3,2,4,
		5,6,7,
		5,7,3,
		7,1,5

	};
	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); i++) {

		glVertex3fv(vertices[indices[i]].ptr());

	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();
}

void ModuleRenderer3D::CreateMainBuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create a Texture Attachment (Texture Color Buffer [TCB])

	glGenTextures(1, &TCB);
	glBindTexture(GL_TEXTURE_2D, TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Cambiado a GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Cambiado a GL_NEAREST
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);

	// Create a Renderbuffer Attachment

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Check Framebuffer Completeness
		// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {

		LOG("Framebuffer is not complete");

	}

	// Bind the Default Framebuffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRenderer3D::RenderMainBuffer(bool toggle)
{
	if (toggle) {

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
	else {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

}

void ModuleRenderer3D::DeleteMainBuffer()
{
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &TCB);
	glDeleteFramebuffers(1, &FBO);
}

void ModuleRenderer3D::BindBuffers()
{
	//Todo: Hacer binnd de todos los meshes arreglar
	for (int i = 0; i < App->mesh->ourMeshes.size(); i++) {

		glGenBuffers(1, &App->mesh->ourMeshes[i]->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, App->mesh->ourMeshes[i]->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ModuleMesh::Vertex) * App->mesh->ourMeshes[i]->ourVertex.size(), &App->mesh->ourMeshes[i]->ourVertex[0], GL_STATIC_DRAW);

		glGenBuffers(1, &App->mesh->ourMeshes[i]->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->mesh->ourMeshes[i]->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * App->mesh->ourMeshes[i]->indices.size(), &App->mesh->ourMeshes[i]->indices[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}