#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"
#include "ModuleScene.h"

#include "..\External\ImGui\imgui.h"
#include "..\External\ImGui/backends/imgui_impl_opengl3.h"
#include "..\External\ImGui/backends/imgui_impl_sdl2.h"
#include "..\External\SDL/include/SDL.h"
#include "GameObject.h"
#include "Component.h"

#include <fstream>



ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app,start_enabled)
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

	return true;
}

void ModuleEditor::DrawEditor()
{
    
    //Todo: Reorganize and check memory leak

    //Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);


    CreateGameObject();
    //Done
    HierarchyWindow();
    InspectorWindow();
   
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
                    // Obtener el nombre de la CPU
                    const char* cpuModel;// = SDL_GetCPUName();
                    cpuModel = "CPU";

                    // Obtener el tamaño de línea de caché de la CPU
                    int cacheLineSize = SDL_GetCPUCacheLineSize();

                    // Obtener el número de núcleos de la CPU
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
                    
                }
            }
        }
        ImGui::End();
    }
    if (ImGui::Begin("Consol"))
    {
       App->console->UpdateConsole();
    }
    ImGui::End();
    
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
    
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
            App->mesh->LoadMesh("../Assets/Cube.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Shpere"))
        {
            LOG("Create a Shpere GameObject");
            App->mesh->LoadMesh("../Assets/Shpere.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Cone"))
        {
            LOG("Create a Cone GameObject");
            App->mesh->LoadMesh("../Assets/Cone.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Cylinder"))
        {
            LOG("Create a Cylinder GameObject");
            App->mesh->LoadMesh("../Assets/Cylinder.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Torus"))
        {
            LOG("Create a Torus GameObject");
            App->mesh->LoadMesh("../Assets/Torus.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Plane"))
        {
            LOG("Create a Plane GameObject");
            App->mesh->LoadMesh("../Assets/Plane.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Polygon"))
        {
            LOG("Create a Polygon GameObject");
            App->mesh->LoadMesh("../Assets/Polygon.fbx");
            App->renderer3D->BindBuffers();

        }
        if (ImGui::MenuItem("Pikachu"))
        {
            LOG("Create a Pikachu GameObject");
            App->mesh->LoadMesh("../Assets/Pikachu.fbx");
            App->texture->LoadTexture("../Assets/Pikachu-Texture.png");
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

            ImGui::TextColored({ 255,255,0,255 }, "Amarillo Engine v0.1");

            std::string licenseContent = loadFile("../../../LICENSE");
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
            ShellExecute(NULL, "open", "https://github.com/MarcelSunyer/Game_Engine", 0, 0, SW_SHOWNORMAL);
        }
        ImGui::EndMenu();
    }

    if (ImGui::Button("Exit"))
    {
        //Exit game
        App->input->quit = true;
    }
   
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
        if (selected_object != NULL)
        {
            ImGui::Begin("Inspector", &show_inspector_window);
            ImGui::Text(selected_object->mName.c_str());
            ImGui::Separator();

            for (uint m = 0; m < selected_object->components.size(); m++)
            {
                if (selected_object->selected)
                {
                    selected_object->components[m]->OnEditor();
                }

            }

            ImGui::End();
        }
    }
}

void ModuleEditor::HierarchyWindow()
{
    ImGui::Begin("Hierarchy");

    DrawHierarchyLevel();

    ImGui::End();
    
}

void ModuleEditor::DrawHierarchyLevel()
{
    std::vector<GameObject*> list2 = App->scene->GetGameObjects();

    for (uint n = 0; n < list2.size(); n++)
    {
        const char* write = list2[n]->mName.c_str();

            // Determine if this GameObject is selected
            bool isSelected = selected_object == list2[n];

            if (ImGui::TreeNode(list2[n]->mName.c_str()))
            {
                // AddGameObjectChilds
                if (ImGui::TreeNode(list2[n]->mName.c_str()))
                {
                    list2[n]->selected = true;
                    selected_object = list2[n];
                        
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        

        // Handle moving the selected GameObject
        if (list2[n]->selected)
        {
            if (ImGui::IsMouseDragging(0))
            {
                //// Update the position of the selected GameObject
                //list2[n]->position.x += ImGui::GetIO().MouseDelta.x;
                //list2[n]->position.y += ImGui::GetIO().MouseDelta.y;
            }
        }
    }
}

void ModuleEditor::CreateGameObject()
{
    if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
    {
        App->scene->CreateGameObject("Pedrito");
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


