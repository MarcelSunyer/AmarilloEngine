#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Globals.h"
#include <vector>
#include "..\External\ImGui/imgui.h"
#include "Primitive.h"
#include <iostream>
#include <string> 
#include "GameObject.h"
#include "../External/ImGuizmo/ImGuizmo.h"
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

enum Timer_State {
	STOPPED,
	RUNNING,
	PAUSED
};

#define MAX_NAME_CHARACTERS 32
class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	update_status Update(float dt);
	bool CleanUp() override;

	void AddFPS(float FPS);

	void ShowAboutInfo();

	void AdjustBrightness(float factor);	

	void AddLog(std::string msg);

	bool StyleTypes(const char* label);

	void InspectorWindow();
	bool show_inspector_window = true;

	void DrawHierarchyLevel(GameObject* currentObject, int num);
	void DrawHierarchy();

	void DrawFolderTree(const std::string& folderPath);

	void OnClick(float pos_x, float pos_y);



	std::string loadFile(const char* filename);
	
	GameObject* GameObject_selected = nullptr;
	GameObject* hoveredGameObj = nullptr;
	GameObject* draggedGameObject = nullptr;
	
	std::vector<float> vecFPSLog;
	const char* lastValue = "";

public:  //Windows Settings
	//Brightness value
	float brightnessFactor = 1.5f;


	//Show config window
	bool showWindow = true;
	bool showAddComponent = false;
	bool showConsole = false;
	char newName[MAX_NAME_CHARACTERS];
public:
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;
	bool IoConfirmer = false;

	int logs_array_size;

	bool wireButton = false;

	//Timer
	Uint32 elapsed_time;
	Timer timer;
	Timer_State timerState = Timer_State::STOPPED;

	//Guizmo
	ImVec2 size_texture_scene;
	ImVec2 windowPosition;
	int offset;

	//Assets Tab
	bool showModal = false;
	std::string selectedFilePath;

	//Size Editor Window
	ImVec2 size_editor;
	ImVec2 size_game;

	ImVec2 position_editor;
};

#endif // MODULE_EDITOR

