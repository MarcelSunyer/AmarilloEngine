#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"
#include <string.h>
#include "..\External\ImGui/imgui.h"
#include "ModuleConsole.h"

#include "..\External\MathGeoLib/include/Math/float3x3.h"
#include "..\External\MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include "LoadFBX.h"
#include "Mesh.h"

#include "..\External\Devil/include/ilut.h"
#include "..\External\Devil/include/ilu.h"
#include "..\External\Devil/include/il.h"

#pragma once (lib, "Devil/libx86/DevIl.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")


//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true); 
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void InitDevil();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;
	Cube juan;
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;

	LoadFBX* myModel = nullptr;
	Primitive* PrimitiveTest = nullptr;

	const char* myModel_path = nullptr;
	const char* myModel_texture_path = nullptr;

	uint VBO;
	uint EBO;
	uint VAO;

	bool activeWire = false;
	bool activeNormals = false;

	
};