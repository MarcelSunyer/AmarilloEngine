#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"
#include "Globals.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include "../External/ImGuizmo/ImGuizmo.h"

#include "..\External\ImGui\imgui.h"
#include "..\External\ImGui/backends/imgui_impl_opengl3.h"
#include "..\External\ImGui/backends/imgui_impl_sdl2.h"
#include "..\External\SDL/include/SDL.h"
#include "GameObject.h"

#include "Component.h"

#include <fstream>
#include <map>
#include "ResourceManager.h"



ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app,start_enabled), elapsed_time(0U)
{

}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        IoConfirmer = true;
    }

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    InitializeIconMapping();

    scriptEditor.Init();

	return true;
}

void ModuleEditor::DrawEditor()
{

    //Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);


    ImGuizmo::Enable(true);

    //Tables & Columnes, row height
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Hierarchy");

    DrawHierarchy();

    ImGui::End();

    if (right_click == true)
    {
        MouseWindow();
    }

    ImGui::Begin("Resources");

    std::map<ResourceType, std::vector<Resource*>> resources = App->resourceManager->GetResources();

    for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
    {
        for (std::vector<Resource*>::iterator re = (*it).second.begin(); re != (*it).second.end(); ++re)
        {
            std::string string = uuids::to_string<char>((*re)->GetUniqueId());

            if ((*it).first == ResourceType::RT_MESH)
            {
                ImGui::Text("Mesh Loaded");
                ImGui::Text("Uid: %s", string.c_str());
                ImGui::Text("");
            }
            if ((*it).first == ResourceType::RT_TEXTURE)
            {
                ImGui::Text("Texture Loaded");
                ImGui::Text("Uid: %s", string.c_str());
                ImGui::Text("");
            }
           
        }
    }
    
    ImGui::End();

    InspectorWindow();
    if (ImGui::Begin("Game"), true) {

        size_game = ImGui::GetContentRegionAvail();
        App->renderer3D->OnResizeGame(size_game.x, size_game.y);
        ImGui::Image((ImTextureID)App->renderer3D->GetGameRenderTexture(), size_game, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
    if (ImGui::Begin("Scene"),true) {

        size_editor = ImGui::GetContentRegionAvail();
        position_editor = ImGui::GetWindowPos();
        ///Guizmos things
        size_texture_scene = size_editor;
        windowPosition = ImGui::GetWindowPos();
        offset = ImGui::GetFrameHeight() / 2;
        ///

        App->renderer3D->OnResize(size_editor.x, size_editor.y);
        ImGui::Image((ImTextureID)App->renderer3D->GetSceneRenderTexture(), size_editor, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
   
    if (showWindow)
    {
        if (ImGui::Begin("Config", &showWindow))
        {
            if (ImGui::CollapsingHeader("Info"))
            {
                ImGui::TextColored({ 255,255,0,255 }, "FPS & Delta Time");
                ImGui::Separator();
                if (ImGui::MenuItem("Delta Time"));
                {
                    // Crear un nuevo vector para almacenar los valores elevados a la potencia de -1
                    std::vector<float> inverseFPSLog;
                    for (float fps : vecFPSLog)
                    {
                        inverseFPSLog.push_back(1.0f / fps);
                    }

                    ImGui::PlotHistogram("dt", inverseFPSLog.data(), inverseFPSLog.size(), 2, lastValue);
                }
                if (ImGui::MenuItem("FPS"));
                {
                    ImGui::PlotHistogram("fps", vecFPSLog.data(), vecFPSLog.size(), 2, lastValue);
                }
                ImGui::TextColored({ 255,255,0,255 }, "PC Specs");
                ImGui::Separator();
                if (ImGui::MenuItem("Graphic Card:"));
                {
                    ImGui::TextColored({ 255,255,0,255 }, "%s", glGetString(GL_RENDERER));
                    ImGui::Text("%s", glGetString(GL_VENDOR));
                    ImGui::Text("%s", glGetString(GL_VERSION));

                }
                if (ImGui::MenuItem("Memory:"));
                {
                    int total_ram_mb = SDL_GetSystemRAM();
                    if (total_ram_mb > 0) {
                        float total_ram_gb = total_ram_mb / 1024.0f;
                        ImGui::Text("Total RAM: %.2f GB", total_ram_gb);
                    }
                    else {
                        ImGui::Text("Error al obtener la memoria RAM total");
                    }

                }
                if (ImGui::MenuItem("CPU:"));
                {

                    const char* cpuModel;
                    cpuModel = "CPU";

                    
                    int cacheLineSize = SDL_GetCPUCacheLineSize();

                    
                    int cpuCount = SDL_GetCPUCount();

                    ImGui::TextColored({ 255,255,0,255 }, "%s", cpuModel);
                    ImGui::Text("CPU Cache Line Size: %d bytes", cacheLineSize);
                    ImGui::Text("CPU Core Count: %d", cpuCount);
                }
            }

            if (ImGui::CollapsingHeader("Window Settings"))
            {
                if (ImGui::CollapsingHeader("Style Options"))
                {
                    ImGuiStyle& style = ImGui::GetStyle();
                    static ImGuiStyle ref_saved_style;

                    if (ImGui::ShowStyleSelector("Colors##Selector"))
                        ref_saved_style = style;
                }
                if (ImGui::CollapsingHeader("Render Options"))
                {
                    if (ImGui::Checkbox("Wireframe", &App->renderer3D->activeWire))
                    {
                    }

                    if (ImGui::Checkbox("Normals", &App->renderer3D->activeNormals))
                    {
                    }

                    if(ImGui::Checkbox("FrustumCulling on editor", &App->renderer3D->activeCullingOnEditorCamera))
                    {

                    }
                }
            }
        }
        ImGui::End();
    }
    if (ImGui::Begin("Console"))
    {
       App->console->UpdateConsole();
    }
    ImGui::End();

    if (ImGui::Begin("Script Editor"))
    {
        scriptEditor.Update();
    }
    ImGui::End();

    if (ImGui::Begin("Time Control"))
    {
        ImGui::Text("                                                   ");
        ImGui::SameLine();

        // Utiliza ImGui::ImageButton para el botón Play
        if (ImGui::ImageButton((void*)App->texture->LoadTexture("../Assets/Editor/play.dds")->textID, ImVec2(32, 32)) && (timerState == Timer_State::PAUSED || timerState == Timer_State::STOPPED))
        {
            timer.Start();
            timerState = Timer_State::RUNNING;
            App->scene->SaveScene();
            App->scene->StartPlay();
            LOG("Start Timer %u ", elapsed_time);
        }
        ImGui::SameLine();

        // Utiliza ImGui::ImageButton para el botón Pause
        if (ImGui::ImageButton((void*)App->texture->LoadTexture("../Assets/Editor/pause.dds")->textID, ImVec2(32, 32)) && timerState == Timer_State::RUNNING)
        {
            timer.Pause();
            timerState = Timer_State::PAUSED;
        }
        ImGui::SameLine();

        // Utiliza ImGui::ImageButton para el botón Stop
        if (ImGui::ImageButton((void*)App->texture->LoadTexture("../Assets/Editor/stop.dds")->textID, ImVec2(32, 32)) && (timerState == Timer_State::RUNNING || timerState == Timer_State::PAUSED))
        {
            timer.Stop();
            elapsed_time = 0;
            timerState = Timer_State::STOPPED;
            App->scene->LoadScene(true);
            LOG("Stop Timer: %u ", elapsed_time);
        }
        ImGui::SameLine();

        switch (timerState)
        {
        case STOPPED:
            ImGui::Text("Current State: STOPPED");
            break;
        case RUNNING:
            ImGui::Text("Current State: RUNNING");
            break;
        case PAUSED:
            ImGui::Text("Current State: PAUSED");
            break;
        }
        ImGui::SameLine();
        ImGui::Text("Current Time: %u ", elapsed_time);

    }
    ImGui::End();


    //ShowAssetBrowser("../Assets/");

#ifdef _DEBUG  // Debug
    ShowAssetBrowser("../Debug/Library/");
#else  // Release
    ShowAssetBrowser("../Release/Library/");
#endif // _DEBUG


    
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
        {
            App->scene->SaveScene();
        }

        if (ImGui::MenuItem("Load Scene", "Stop Button"))
        {
            App->scene->LoadScene(true);
        }

        if (ImGui::MenuItem("Load Basic Scene", "Ctrl+L"))
        {
            App->scene->LoadScene(false);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("GameObjects"))
    {
        if (ImGui::MenuItem("Cube"))
        {
            LOG("Create a Cube GameObject");
            App->mesh->LoadMesh("../Assets/Models/Cube.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Shpere"))
        {
            LOG("Create a Shpere GameObject");
            App->mesh->LoadMesh("../Assets/Models/Sphere.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Cone"))
        {
            LOG("Create a Cone GameObject");
            App->mesh->LoadMesh("../Assets/Models/Cone.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Cylinder"))
        {
            LOG("Create a Cylinder GameObject");
            App->mesh->LoadMesh("../Assets/Models/Cylinder.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Torus"))
        {
            LOG("Create a Torus GameObject");
            App->mesh->LoadMesh("../Assets/Models/Torus.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Plane"))
        {
            LOG("Create a Plane GameObject");
            App->mesh->LoadMesh("../Assets/Models/Plane.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Polygon"))
        {
            LOG("Create a Polygon GameObject");
            App->mesh->LoadMesh("../Assets/Models/Polygon.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Pikachu"))
        {
            LOG("Create a Pikachu GameObject");
            App->scene->LoadMeshAndTexture("../Assets/Models/Pikachu.fbx", "../Assets/Textures/Pikachu-Texture.png");

            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Skybox"))
        {
            LOG("Create a Skybox GameObject");
            App->scene->LoadMeshAndTexture("../Assets/Models/Skybox.fbx", "../Assets/Textures/skybox.png");
            App->renderer3D->BindBuffers();

        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window"))
    {
        
        //Shows the nconfig windows
        if (ImGui::Button("Config"))
        {
            showWindow = true;
        }

        ImGui::Text("Computer Window Size: ");
        ImGui::Text("Width: %dpx  Height: %dpx", SCREEN_WIDTH, SCREEN_HEIGHT);
        
        ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::BeginMenu("About"))
        {

            ImGui::TextColored({ 255,255,0,255 }, "Amarillo Engine v0.5");

            std::string licenseContent = loadFile("../../LICENSE");
            if (!licenseContent.empty())
            {
                
                ImGui::PushTextWrapPos(500.0f);
                ImGui::TextWrapped("%s", licenseContent.c_str());
               
                ImGui::PopTextWrapPos();
            }
           
            ImGui::Text("USER GUIDE:");
            ImGui::ShowUserGuide();

            ImGui::EndMenu();
        }
      
        if (ImGui::MenuItem("GitHub"))
        {
            ShellExecute(NULL, "open", "https://github.com/MarcelSunyer/AmarilloEngine", 0, 0, SW_SHOWNORMAL);
        }
        ImGui::EndMenu();
    }

    if (ImGui::Button("Exit"))
    {
        //Exit game
        App->input->quit = true;
    }

    App->scene->ImGuizmoHandling();
   
    ImGui::EndMainMenuBar();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (IoConfirmer)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

update_status ModuleEditor::Update(float dt)
{
    //Arreglar mouse picking con el gizmo
    if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
    {
        OnClick(App->input->GetMouseX(), App->input->GetMouseY());
    }

    return update_status::UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();


	return true;
}

void ModuleEditor::AddFPS(float FPS)
{
    if (vecFPSLog.size() < 100)
    {
        vecFPSLog.push_back(1/FPS);
    }
    else
    {
        vecFPSLog.erase(vecFPSLog.begin());
        vecFPSLog.push_back(1/FPS);
    }
}

void ModuleEditor::ShowAboutInfo()
{
  
}
void ModuleEditor::AdjustBrightness(float factor)
{
    // Calculate brightness-adjusted colors
    ImVec4 textColor = ImVec4(1.0f * factor, 1.0f * factor, 1.0f * factor, 1.0f);
    ImVec4 bgColor = ImVec4(0.2f * factor, 0.2f * factor, 0.2f * factor, 1.0f);

    // Set ImGui style colors
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = textColor;
    style.Colors[ImGuiCol_WindowBg] = bgColor;
}

void ModuleEditor::AddLog(std::string msg)
{
    App->console->AddLog(msg);
}

bool ModuleEditor::StyleTypes(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Dark\0Light\0Classic\0"))
    {
        switch (style_idx)
        {
        case 0: ImGui::StyleColorsDark(); break;
        case 1: ImGui::StyleColorsLight(); break;
        case 2: ImGui::StyleColorsClassic(); break;
        }
        return true;
    }
    return false;
}

void ModuleEditor::InspectorWindow()
{
    if (show_inspector_window)
    {
        ImGui::Begin("Inspector", &show_inspector_window);

        if (GameObject_selected != nullptr)
        {
            ImGui::Checkbox("Active",&GameObject_selected->active);
            ImGui::SameLine;
            strcpy(newName, GameObject_selected->mName.c_str());
            if (ImGui::InputText("##test_1", &newName[0], sizeof(newName)))
            {
                    GameObject_selected->mName = newName;
            }
           
            if (ImGui::Button("Delete")) {

                App->scene->DeleteGameObject(GameObject_selected);

            }
           
            ImGui::Separator();
            if (GameObject_selected != nullptr)
            {
                for (uint m = 0; m < GameObject_selected->components.size(); ++m)
                {
                    GameObject_selected->components[m]->OnEditor();
                }
            }
          
            ImGui::Dummy(ImVec2(0,15));
            ImGui::Text("     ");
            ImGui::SameLine();
            // Inicia el combo
            if (ImGui::BeginCombo("##test_2","AddComponent"))
            {
                showAddComponent = true;
                if (showAddComponent)
                {//Pregunta profe
                    if (ImGui::Selectable("ComponentTexture"))
                    {
                        if (GameObject_selected->GetComponent(ComponentTypes::TEXTURE) == nullptr)
                        {
                            GameObject_selected->AddComponent(ComponentTypes::TEXTURE);
                        }

                    }
                    if (ImGui::Selectable("ComponentCamera"))
                    {
                        if (GameObject_selected->GetComponent(ComponentTypes::CAMERA) == nullptr)
                        {
                            GameObject_selected->AddComponent(ComponentTypes::CAMERA);
                        }

                    }
                    if (ImGui::Selectable("ComponentScript"))
                    {
                        if (GameObject_selected->GetComponent(ComponentTypes::SCRIPT) == nullptr)
                        {
                            GameObject_selected->AddComponent(ComponentTypes::SCRIPT);
                        }

                    }
                }
                // Finaliza el combo
                ImGui::EndCombo();
            }
        }

        ImGui::End();
    }



}


void ModuleEditor::DrawHierarchyLevel(GameObject* currentObject, int num)
{   
    ImGuiTreeNodeFlags flag_TNode = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;

    bool isNodeOpen;
    bool isSelected = GameObject_selected == currentObject;

    if (currentObject->children.size() != 0){
        isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)num, flag_TNode, currentObject->mName.c_str(), num);
    }

    else{
        flag_TNode |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)num, flag_TNode, currentObject->mName.c_str(), num);
        isNodeOpen = false;
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", currentObject, sizeof(GameObject*));
        draggedGameObject = currentObject;
        ImGui::Text("Dragging GameObject");
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
    {
        hoveredGameObj = currentObject;
        if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            GameObject_selected = currentObject;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            right_click = true;
        }
        else
        {
            right_click = false;
        }
    }
    

    if (ImGui::IsWindowHovered())
    {
        if (!ImGui::IsAnyItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                GameObject_selected = nullptr;
            }
           
        }
       
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {

            draggedGameObject->SetParent(hoveredGameObj);

        }
        ImGui::EndDragDropTarget();
    }

    if (isNodeOpen)
    {
        if (!currentObject->children.empty()) {
            for (unsigned int i = 0; i < currentObject->children.size(); ++i)
            {
                DrawHierarchyLevel(currentObject->children[i], i);
            }
        }
        ImGui::TreePop();
    }

}

void ModuleEditor::DrawHierarchy()
{
    std::vector<GameObject*> lista_games = App->scene->root_object->children;

    for (uint i = 0; i < lista_games.size(); ++i)
    {
        DrawHierarchyLevel(lista_games[i],i);
    }

}

std::string ModuleEditor::loadFile(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        LOG("Error: No se pudo abrir el archivo %s\n", filename);
        return std::string();
    }

    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void ModuleEditor::InitializeIconMapping() {

    iconMapping[".png"] = "../Assets/Editor/image.dds";
    iconMapping[".ico"] = "../Assets/Icon.ico";
    iconMapping[".fbx"] = "../Assets/Editor/model.dds";
    iconMapping[".dds"] = "../Assets/Editor/image.dds";
    iconMapping[".ascene"] = "../Assets/Editor/scene.dds";
    iconMapping[".meta"] = "../Assets/Editor/files.dds";
    iconMapping[".folder"] = "../Assets/Editor/folder.dds";
    iconMapping[".prefab"] = "../Assets/Editor/prefab.dds";
}

void ModuleEditor::OpenAsset(const std::string& assetPath) {
    
    const char* ext = strrchr(assetPath.c_str(), '.');

    if (ext != NULL) {
        if (strcmp(ext, ".fbx") == 0 || strcmp(ext, ".FBX") == 0 || strcmp(ext, ".dae") == 0 || strcmp(ext, ".DAE") == 0) {
            // File extension FBX or DAE: Load Mesh
            LOG(("Mesh opened: %s", assetPath.c_str()));

            App->mesh->LoadMesh(assetPath.c_str());
            App->renderer3D->BindBuffers();
        }
        else if (strcmp(ext, ".png") == 0 || strcmp(ext, ".tga") == 0 || strcmp(ext, ".dds") == 0) {
            //Texture file, load texture to selected object
            LOG(("Texture opened: %s", assetPath.c_str()));
            if (GameObject_selected != nullptr)
            {
                if ((ComponentTexture*)GameObject_selected->GetComponent(ComponentTypes::TEXTURE) != nullptr)
                {
                    //GameObject_selected->texture->texture = App->texture->LoadTexture(assetPath);
                    ComponentTexture* tmp_Component = (ComponentTexture*)GameObject_selected->GetComponent(ComponentTypes::TEXTURE);
                    tmp_Component->SetTexture(App->texture->LoadTexture(assetPath));
                }
            }
        }
        else if (strcmp(ext, ".ascene") == 0) {
            //Scene file, load scene (de momento solo carga la escena "Base_Layout.ascene")
            App->scene->LoadScene(false);

        }
        else {
            LOG(("Unsupported file type: %s", assetPath.c_str()));
        }
    }
    else {
        LOG(("Unable to determine file extension: %s", assetPath.c_str()));
    }
}

void ModuleEditor::DrawAsset(const std::string& assetPath, int column) {
    // Get file extension
    std::string extension = std::filesystem::path(assetPath).extension().string();

    // Busca la extensión en el mapeo de iconos
    auto iconIt = iconMapping.find(extension);
    if (iconIt != iconMapping.end()) {
        // Comprueba si la textura ya está cargada
        auto textureIt = loadedTextures.find(iconIt->second);
        if (textureIt == loadedTextures.end() || !textureIt->second.loaded) {
            // Si no está cargada, carga la textura y actualiza la información en el mapa
            Texture* texture = App->texture->LoadTextureEditor(iconIt->second.c_str());
            loadedTextures[iconIt->second] = { texture->textID, true };
        }

        // Muestra el icono
        ImGui::TextWrapped("%s", std::filesystem::path(assetPath).filename().string().c_str());
        ImGui::PushID(assetPath.c_str());
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(loadedTextures[iconIt->second].textID)), ImVec2(64, 64));
        

        // Handle double-click action
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            OpenAsset(assetPath);
        }

        ImGui::PopID();
    }
    else {
        // Si no se encuentra un icono específico, carga un icono predeterminado o muestra el nombre del archivo
        ImGui::TextWrapped("%s", std::filesystem::path(assetPath).filename().string().c_str());

        // Resto del código...
    }
}

void ModuleEditor::DrawFolderContents(const std::string& folderPath, std::vector<std::string>& currentPath) {
    const int numColumns = 5;
    const float iconSize = 80.0f;
    const float spacing = 10.0f; // Espacio entre iconos

    if (ImGui::BeginTable("table2", numColumns))
    {
        int column = 0;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            ImGui::TableNextColumn();

            if (entry.is_directory()) {
                // Draw folder title
                ImGui::Text("%s", entry.path().filename().string().c_str());
                ImGui::Separator();

                // Busca la extensión en el mapeo de iconos
                auto iconIt = iconMapping.find(".folder");
                if (iconIt != iconMapping.end()) {
                    // Comprueba si la textura ya está cargada
                    auto textureIt = loadedTextures.find(iconIt->second);
                    if (textureIt == loadedTextures.end() || !textureIt->second.loaded) {
                        // Si no está cargada, carga la textura y actualiza la información en el mapa
                        Texture* texture = App->texture->LoadTextureEditor(iconIt->second.c_str());
                        loadedTextures[iconIt->second] = { texture->textID, true };
                    }

                    // Muestra el icono para la carpeta
                    ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(loadedTextures[iconIt->second].textID)), ImVec2(64, 64));
                }

                // Check for folder click
                if (ImGui::IsItemClicked()) {
                    // If folder is clicked, enter into the folder
                    currentPath.push_back(entry.path().filename().string());
                }
            }
            else {
                // DrawAsset with manual positioning in the table cell
                DrawAsset(entry.path().string(), column);
            }

            // Nueva línea después de cada elemento (puedes ajustar según sea necesario)
            if ((column + 1) % numColumns == 0) {
                ImGui::TableNextRow();
            }

            column++;
        }

        ImGui::EndTable();
    }
}


void ModuleEditor::MarkTexturesAsUnloaded() {
    for (auto& texture : loadedTextures) {
        texture.second.loaded = false;
    }
}

void ModuleEditor::ShowAssetBrowser(const char* path) {

    // Obtain name from path
    std::filesystem::path filePath(path);
    std::string folderName = App->file_system->GetFolderNameFromPath(filePath.parent_path().string().c_str());

    std::string name = folderName.empty() ? "Browser" : folderName + " Browser";

    // Asset Browser Window
    ImGui::Begin(name.c_str());

    // Back button
    if (ImGui::Button("Back") && !currentPath.empty()) {

        currentPath.pop_back();
    }

    // Draw assets in the current folder
    //std::string currentFolder = "../Assets/";
    std::string currentFolder = path;
    for (const auto& folder : currentPath) {
        currentFolder = currentFolder + folder + "/";
    }

    DrawFolderContents(currentFolder, currentPath);

    ImGui::End();
}

void ModuleEditor::MouseWindow()
{
   
    float x = App->input->GetMouseX();
    float y = App->input->GetMouseY();
    ImVec2 mouseOos = { x, y };

    ImGui::SetNextWindowPos(mouseOos);

    ImGui::Begin("##Juan", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("Delete GameObject"))
    {
        GameObject_selected->Disable();;
    }
    ImGui::End();
}

void ModuleEditor::OnClick(float pos_x, float pos_y)
{
    pos_x -= position_editor.x;
    pos_y -= position_editor.y;

    float normalPos_x = pos_x / size_editor.x;
    float normalPos_y = 1 - (pos_y / size_editor.y);

    normalPos_x = (2 * normalPos_x) - 1;
    normalPos_y = (2 * normalPos_y) - 1;
    
    LineSegment latest_ray = App->camera->editor_camera->Camera_frustum.UnProjectLineSegment(normalPos_x, normalPos_y);

    applic->scene->TestGameObjectSelection(latest_ray);
}
