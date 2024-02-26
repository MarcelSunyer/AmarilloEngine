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
#include "../External/ImGuizmo/ImGuizmo.h"
#include <cstring>
#include <filesystem>
#include <unordered_map>
#include "ScriptEditor.h"

namespace fs = std::filesystem;

class GameObject;

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


	void OnClick(float pos_x, float pos_y);

	std::string loadFile(const char* filename);

	void InitializeIconMapping();

	void OpenAsset(const std::string& assetPath);

	void DrawAsset(const std::string& assetPath, int column);

	void DrawFolderContents(const std::string& folderPath, std::vector<std::string>& currentPath);

	void MarkTexturesAsUnloaded();

	void ShowAssetBrowser(const char* path);

	void MouseWindow();
	
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

	//Asset explorer
	std::vector<std::string> currentPath;
	std::unordered_map<std::string, std::string> iconMapping;

	struct TextureInfo_E {
		uint textID;
		bool loaded;
	};
	std::unordered_map<std::string, TextureInfo_E> loadedTextures;
	std::unordered_map<std::string, bool> textureLoadedState;

	bool right_click = false;

	ScriptEditor scriptEditor;
};

#endif // MODULE_EDITOR

