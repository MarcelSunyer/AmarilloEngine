#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl2.h"


ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app,start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    /*
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    */

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(this->App->window->window, this->App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

	return false;
}

void ModuleEditor::DrawEditor()
{
    //Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    //ImGui::NewFrame();
    
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::BeginMenu("About"))
        {
            ImGui::Text("Amarillo Engine v0.1");
            ImGui::BulletText("By Eric Luque & Marcel Sunyer");
            ImGui::Text("3rd Party Libraries used:");
            ImGui::BulletText("SDL2");
            ImGui::BulletText("OpenGL3");
            ImGui::BulletText("Glew");
            ImGui::BulletText("MathGeoLib");
            ImGui::BulletText("ImGui");

            ImGui::Text("MIT License:");
            ImGui::Text("Copyright (c) 2023 Sergi Perez & Adria Martin");
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
        ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();


	return false;
}

