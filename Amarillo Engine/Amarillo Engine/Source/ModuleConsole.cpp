#include "ModuleConsole.h"
#include "Application.h"
#include "..\External\ImGui\imgui.h"

ModuleConsole::ModuleConsole(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleConsole::~ModuleConsole()
{
}


void ModuleConsole::UpdateConsole()
{
    for (int i = 0; i < logs_array_size; i++)
    {
        ImGui::Text("%s", logs_array[i].logs.c_str());
        if (logs_array[i].rep > 1)
        {
            ImGui::SameLine();
            ImGui::Text("%d", logs_array[i].rep);
        }
    }
}

void ModuleConsole::AddLog(std::string msg)
{
    //LOG(msg.c_str());
    // Mirar si el array logs_array no está vacío
    if (logs_array_size > 0)
    {
        // Obtener el último log
        Logs& last_log = logs_array[logs_array_size - 1];

        // Comparar si el mensaje es igual al último log
        if (msg == last_log.logs)
        {
            // Incrementar la cuenta de repeticiones
            last_log.rep++;
            return;
        }
    }

    // Si no es igual, agregar un nuevo log al array
    Logs new_log;
    new_log.logs = msg;
    new_log.rep = 1;
    logs_array[logs_array_size] = new_log;
    logs_array_size++;
}
