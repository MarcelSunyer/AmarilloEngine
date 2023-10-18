#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"
#include "Imgui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
//#include "ImGui/backends/imgui_impl_opengl3_loader.h"
#include "SDL/include/SDL.h"
#include <iostream>


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
   
    


    //Show demo
    ImGui::ShowDemoWindow();
   
    if (showWindow)
    {
        if (ImGui::Begin("Config", &showWindow))
        {
            MovingTabWindow();
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
                    
                }
            }
        }
        ImGui::End();
       
    }
    if (ImGui::Begin("Consol"))
    {
        MovingTabWindow();

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

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window"))
    {
        
        //Shows the nconfig windows
        if (ImGui::Button("Config"))
        {
            MovingTabWindow();
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
            ImGui::BulletText("By Eric Luque & Marcel Sunyer");
            ImGui::Separator();

            ImGui::Text("3rd Party Libraries used:");
            ImGui::BulletText("SDL2");
            ImGui::BulletText("OpenGL3");
            ImGui::BulletText("Glew");
            ImGui::BulletText("MathGeoLib");
            ImGui::BulletText("ImGui");
            ImGui::Separator();

            ImGui::Text("MIT License:");
            ImGui::Text("Copyright (c) 2023 Eric Luque & Marcel Sunyer");
            ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this software");
            ImGui::Text("and associated documentation files(the 'Software'), to deal in the Software without ");
            ImGui::Text("restriction, including without limitation the rights to use, copy, modify, merge, publish, ");
            ImGui::Text("distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the");
            ImGui::Text("Software is furnished to do so, subject to the following conditions:");
            ImGui::Text("The above copyright notice and this permission notice shall be included in all copies or ");
            ImGui::Text("substantial portions of the Software.");
            ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS");
            ImGui::Text("OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF");
            ImGui::Text("MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND");
            ImGui::Text("NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS");
            ImGui::Text("BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN");
            ImGui::Text("ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN");
            ImGui::Text("CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
            ImGui::Text("SOFTWARE.");
            ImGui::Separator();

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

void ModuleEditor::MovingTabWindow()
{
    // Todo: Arreglar esto

    if (ImGui::IsWindowHovered())
    {
        if (ImGui::IsMouseClicked(0)) 
        {
            App->camera->windowMovement = true;
        }

    }
    else
    {
        AddLog("jUAN");
        App->camera->windowMovement = false;
    }
}


