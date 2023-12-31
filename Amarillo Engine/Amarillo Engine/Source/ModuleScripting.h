#ifndef MODULE_SCRIPTING
#define MODULE_SCRIPTING
#include "../External/AngelScript/sdk/angelscript/include/angelscript.h"
#include <../../../Amarillo Engine/External/AngelScript/sdk/add_on/scriptbuilder/scriptbuilder.h>
#include <../../../Amarillo Engine/External/AngelScript/sdk/add_on/scriptstdstring/scriptstdstring.h>

#include <iostream> 
#include <assert.h>  
#include <string.h>  

#include <conio.h>   
#include <windows.h> 

#include "Module.h"

using namespace std;

class ModuleScripting : public Module
{
public:
	ModuleScripting(Application* app, bool start_enabled = true);

	~ModuleScripting();

	bool Init() override;
	update_status Update(float dt);
	bool CleanUp();

	void ConfigureEngine(asIScriptEngine* engine);
	int CompileScript(asIScriptEngine* engine);


};

#endif //MODULE_SCRIPTING