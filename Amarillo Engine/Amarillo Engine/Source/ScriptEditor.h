#ifndef __SCRIPT_EDITOR__
#define __SCRIPT_EDITOR__

#include "../External/ImGuiColorTextEditor/TextEditor.h"


#include "../External/ImGui/imgui.h"


#include <string>
#include <filesystem>


class ScriptEditor {
public:

	ScriptEditor();
	virtual ~ScriptEditor();

	bool Init();
	bool Update();

	//Load code into the text editor
	void LoadScriptTXT(std::string filePath);

private:

	// Script File Management

	void CreateScriptTXT();
	bool SaveScriptTXT(std::string shaderText, std::string fileName);
	void DeleteScriptTXT(std::string fileName);

private:

	TextEditor textEditor;
	TextEditor::LanguageDefinition codeLanguage;

	std::string scriptName;

	std::string baseCode = "//Codigo generico de C++";

};

#endif // __SCRIPT_EDITOR__
