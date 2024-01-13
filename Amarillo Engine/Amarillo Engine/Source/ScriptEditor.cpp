#include "ScriptEditor.h"

ScriptEditor::ScriptEditor()
{
	codeLanguage = TextEditor::LanguageDefinition::CPlusPlus();
	textEditor.SetLanguageDefinition(codeLanguage);

	scriptName = "Script";
}

ScriptEditor::~ScriptEditor()
{
}

bool ScriptEditor::Init()
{
	bool ret = true;

	return ret;
}

bool ScriptEditor::Update()
{
    bool ret = true;

    // File Name Input with yellow background
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "File Name: ");
    ImGui::SameLine();

    char nameBuffer[256];
    strcpy(nameBuffer, scriptName.c_str());

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 0.0f, 0.2f)); // Yellow background
    if (ImGui::InputText("##FileName", nameBuffer, sizeof(nameBuffer)))
    {
        scriptName = nameBuffer;
    }
    ImGui::PopStyleColor();

    ImGui::Separator();

    // Script Editor Functionality

    if (ImGui::Button("Create Script", ImVec2(120, 30)))
    {
        CreateScriptTXT();
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Script", ImVec2(120, 30)))
    {
        SaveScriptTXT(textEditor.GetText(), scriptName);
        // Recompile scripts if needed
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete Script", ImVec2(120, 30)))
    {
        DeleteScriptTXT(scriptName);
    }

    ImGui::Separator();

    // Script Editor Rendering
    textEditor.Render("Script Editor");

    return ret;
}


void ScriptEditor::LoadScriptTXT(std::string filePath)
{
}

void ScriptEditor::CreateScriptTXT()
{
	textEditor.SetText(baseCode);
	scriptName = "New Script";
}

bool ScriptEditor::SaveScriptTXT(std::string shaderText, std::string fileName)
{
	return false;
}

void ScriptEditor::DeleteScriptTXT(std::string fileName)
{
}
