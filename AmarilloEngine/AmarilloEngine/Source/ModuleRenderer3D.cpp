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

	for (int i = 0; i < CHECKERS_WIDTH; ++i) {
		for (int j = 0; j < CHECKERS_HEIGHT; ++j) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_2D);

	Grid.axis = true;

	ilInit();


	App->scene->LoadMeshAndTexture("../Assets/Models/Skybox.fbx", "../Assets/Textures/skybox.png")->mName = "Skybox";
	//App->scene->LoadMeshAndTexture("../Assets/Models/BakerHouse.fbx", "../Assets/Textures/Baker_house.png")->mName = "Baker House";
	App->mesh->LoadMesh("../Assets/Street_Environment/Test.fbx")->mName = "Street_Environment";

	//Script FUYM
	App->scene->LoadMeshAndTexture("../Assets/Models/Tank.fbx", "../Assets/Textures/green.png")->mName = "Tank";
	App->scene->CreateGameObject("Lower_Tank", GameObject::FindWithName("Tank"));
	GameObject::FindWithName("TankTracksLeft")->SetParent(GameObject::FindWithName("Lower_Tank"));
	GameObject::FindWithName("TankTracksRight")->SetParent(GameObject::FindWithName("Lower_Tank"));
	GameObject::FindWithName("TankChassis")->SetParent(GameObject::FindWithName("Lower_Tank"));


	SDL_MaximizeWindow(App->window->window);
	BindBuffers();

	CreateMainBuffer();

	return ret;
}

update_status ModuleRenderer3D::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	scene_render_texture->Bind();
	
	RenderFromCamera(App->camera->GetEditorCamera(),true, activeCullingOnEditorCamera);
	//RenderOnFrustrum(App->camera->GetEditorCamera(),App->mesh->globalAABB);
	scene_render_texture->Unbind();
	
	if (App->camera->active_camera != nullptr)
	{
		game_render_texture->Bind();

		RenderFromCamera(App->camera->active_camera,false,true);

		game_render_texture->Unbind();
	}
	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < App->mesh->ourMeshes.size(); ++i) {

		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->VBO);
		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->EBO);
	}
	DeleteMainBuffer();

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	//Falta poner la del game
	App->camera->SetAspectRatio(width, height);
}
//Falta arreglar la camara game
void ModuleRenderer3D::OnResizeGame(int width, int height)
{
	App->camera->SetAspectRatioGame(width, height);
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

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); ++i) {

		glVertex3fv(vertices[indices[i]].ptr());

	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();
}

void ModuleRenderer3D::CreateMainBuffer()
{
	scene_render_texture = new RenderTexture();
	scene_render_texture->Create(SCREEN_WIDTH,SCREEN_HEIGHT);
	
	game_render_texture = new RenderTexture();
	game_render_texture->Create(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ModuleRenderer3D::DeleteMainBuffer()
{
	delete(scene_render_texture);
	delete(game_render_texture);
}

GLuint ModuleRenderer3D::GetSceneRenderTexture()
{
	return scene_render_texture->GetTexture();
}

GLuint ModuleRenderer3D::GetGameRenderTexture()
{
	return game_render_texture->GetTexture();
}

void ModuleRenderer3D::RenderFromCamera(Camera3D* camera, bool debug_draw_enabled, bool useculling)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(camera->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetViewMatrix());


	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	if (debug_draw_enabled)
	{
		//App->mesh->UpdateBoundingBoxes();
		Grid.Render();
		App->scene->DebugDrawGameObjects();

	}
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

	for (uint n = 0; n < gameObject_list.size(); ++n)
	{
		GameObject* gameobject = gameObject_list[n];
		
		if (!gameobject->active)
		{
			continue;
		}

		for (uint m = 0; m < gameobject->components.size(); ++m)
		{
			Component* component = gameobject->components[m];

			if (component->type != ComponentTypes::MESH)
			{
				continue;
			}
			
			ComponentMesh* componentMesh = (ComponentMesh*)component;

			if (useculling) 
			{
				bool isInsideFrustum = RenderOnFrustrum(App->camera->active_camera, componentMesh->globalAABB);

				if (!isInsideFrustum)
				{
					continue;
				}
			}

			float4x4 matrix = float4x4::FromTRS(float3(5, 1, 1), Quat::identity, float3(1, 1, 1));

			glPushMatrix();
			glMultMatrixf(gameobject->transform->GetTransformMatrix().Transposed().ptr());
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_COORD_ARRAY);
			//Bind Mesh
			glBindBuffer(GL_ARRAY_BUFFER, componentMesh->mesh_->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, componentMesh->mesh_->EBO);
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

			glDrawElements(GL_TRIANGLES, componentMesh->mesh_->indices.size(), GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_COORD_ARRAY);
			glPopMatrix();
		}
	}
	if (activeNormals)
	{
		App->mesh->DrawNormals();
	}
}

bool ModuleRenderer3D::RenderOnFrustrum(const Camera3D* camera, const AABB& aabb)
{
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	Plane planes[6];
	camera->Camera_frustum.GetPlanes(planes);

	for (uint i = 0; i < 6; ++i)
	{
		uint point_inside_plane = 8;

		for (uint p = 0; p < 8; ++p)
		{
			if (planes[i].IsOnPositiveSide(corners[p]))
			{
				--point_inside_plane;
			}
		}

		if (point_inside_plane == 0)
		{
			return false;
		}
	}

	return true;

}

void ModuleRenderer3D::DebugDrawBox(const float3* corners, Color color, bool lines, const float& line_size)
{
	GLint previous[2];
	if (lines)
	{
		glGetIntegerv(GL_POLYGON_MODE, previous);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glColor3f(color.r, color.g, color.b);

	glLineWidth(line_size);

	glBegin(GL_LINES);
	
	// Base del cubo
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);

	// Cara frontal del cubo
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[3]);

	// Cara posterior del cubo
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);
	glVertex3fv((GLfloat*)&corners[6]);

	// Cara superior del cubo
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	// Cara inferior del cubo
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	// Cara izquierda del cubo
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[2]);

	// Cara derecha del cubo
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[1]);

	glEnd();

	if (lines)
		glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glLineWidth(1.0f);

	glColor3f(255, 255, 255);
}

void ModuleRenderer3D::DebugDraw(const Frustum& frustum, Color color, bool lines, const float& line_size)
{
		float3 vertices[8];
		frustum.GetCornerPoints(vertices);

		DebugDrawBox(vertices, color, lines, line_size);
}
uint ModuleRenderer3D::LoadBuffer(float* elements, uint size)
{
	uint id = 0;

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * size, elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;

};
uint ModuleRenderer3D::LoadBuffer(uint* elements, uint size)
{

	uint id = 0;

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * size, elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;

};

uint ModuleRenderer3D::LoadTextureBuffer(const void* texture, uint size, int format, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min)
{
	uint id = 0;

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		LOG("%d", err);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(size, (GLuint*)&(id));
	glBindTexture(GL_TEXTURE_2D, id);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		LOG("%d", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		LOG("%d", err);
	}


	return id;
}

void ModuleRenderer3D::UnloadTextureBuffer(uint id, uint size)
{
	glDeleteTextures(size, &id);
}

void ModuleRenderer3D::UnloadBuffer(uint id, uint size)
{
	if (id != 0)
	{
		glDeleteBuffers(1, (GLuint*)&id);
	}
}
void ModuleRenderer3D::BindBuffers()
{
	//Todo: Hacer binnd de todos los meshes arreglar
	for (int i = 0; i < App->mesh->ourMeshes.size(); ++i) {

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
