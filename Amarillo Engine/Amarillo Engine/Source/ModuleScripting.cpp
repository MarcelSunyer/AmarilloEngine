//#include "ModuleScripting.h"
//
//ModuleScripting::ModuleScripting(Application* app, bool start_enabled) : Module(app, start_enabled)
//{
//
//}
//
//ModuleScripting::~ModuleScripting()
//{
//}
//
//bool ModuleScripting::Init()
//{
//
//	int r;
//
//	// Create the script engine
//	asIScriptEngine* engine = asCreateScriptEngine();
//	if (engine == 0)
//	{
//		cout << "Failed to create script engine." << endl;
//		return -1;
//	}
//
//	// The script compiler will write any compiler messages to the callback.
//	engine->SetMessageCallback(asFUNCTION("MessageCallback"), 0, asCALL_CDECL);
//
//	// Configure the script engine with all the functions, 
//	// and variables that the script should be able to use.
//	ConfigureEngine(engine);
//
//	// Compile the script code
//	r = CompileScript(engine);
//	if (r < 0)
//	{
//		engine->Release();
//		return -1;
//	}
//
//	// Create a context that will execute the script.
//	asIScriptContext* ctx = engine->CreateContext();
//	if (ctx == 0)
//	{
//		cout << "Failed to create the context." << endl;
//		engine->Release();
//		return -1;
//	}
//
//	// We don't want to allow the script to hang the application, e.g. with an
//	// infinite loop, so we'll use the line callback function to set a timeout
//	// that will abort the script after a certain time. Before executing the 
//	// script the timeOut variable will be set to the time when the script must 
//	// stop executing. 
//	DWORD timeOut;
//	r = ctx->SetLineCallback(asFUNCTION("LineCallback"), &timeOut, asCALL_CDECL);
//	if (r < 0)
//	{
//		cout << "Failed to set the line callback function." << endl;
//		ctx->Release();
//		engine->Release();
//		return -1;
//	}
//
//	// Find the function for the function we want to execute.
//	asIScriptFunction* func = engine->GetModule(0)->GetFunctionByDecl("float calc(float, float)");
//	if (func == 0)
//	{
//		cout << "The function 'float calc(float, float)' was not found." << endl;
//		ctx->Release();
//		engine->Release();
//		return -1;
//	}
//
//	// Prepare the script context with the function we wish to execute. Prepare()
//	// must be called on the context before each new script function that will be
//	// executed. Note, that if you intend to execute the same function several 
//	// times, it might be a good idea to store the function returned by 
//	// GetFunctionByDecl(), so that this relatively slow call can be skipped.
//	r = ctx->Prepare(func);
//	if (r < 0)
//	{
//		cout << "Failed to prepare the context." << endl;
//		ctx->Release();
//		engine->Release();
//		return -1;
//	}
//
//	// Now we need to pass the parameters to the script function. 
//	ctx->SetArgFloat(0, 3.14159265359f);
//	ctx->SetArgFloat(1, 2.71828182846f);
//
//	// Set the timeout before executing the function. Give the function 1 sec
//	// to return before we'll abort it.
//	timeOut = timeGetTime() + 1000;
//
//	// Execute the function
//	cout << "Executing the script." << endl;
//	cout << "---" << endl;
//	r = ctx->Execute();
//	cout << "---" << endl;
//	if (r != asEXECUTION_FINISHED)
//	{
//		// The execution didn't finish as we had planned. Determine why.
//		if (r == asEXECUTION_ABORTED)
//			cout << "The script was aborted before it could finish. Probably it timed out." << endl;
//		else if (r == asEXECUTION_EXCEPTION)
//		{
//			cout << "The script ended with an exception." << endl;
//
//			// Write some information about the script exception
//			asIScriptFunction* func = ctx->GetExceptionFunction();
//			cout << "func: " << func->GetDeclaration() << endl;
//			cout << "modl: " << func->GetModuleName() << endl;
//			cout << "sect: " << func->GetScriptSectionName() << endl;
//			cout << "line: " << ctx->GetExceptionLineNumber() << endl;
//			cout << "desc: " << ctx->GetExceptionString() << endl;
//		}
//		else
//			cout << "The script ended for some unforeseen reason (" << r << ")." << endl;
//	}
//	else
//	{
//		// Retrieve the return value from the context
//		float returnValue = ctx->GetReturnFloat();
//		cout << "The script function returned: " << returnValue << endl;
//	}
//
//	// We must release the contexts when no longer using them
//	ctx->Release();
//
//	// Shut down the engine
//	engine->ShutDownAndRelease();
//
//	return true;
//}
//
//update_status ModuleScripting::Update(float dt)
//{
//
//	return UPDATE_CONTINUE;
//}
//
//bool ModuleScripting::CleanUp()
//{
//	return true;
//}
//
//void ModuleScripting::ConfigureEngine(asIScriptEngine* engine)
//{
//	int r;
//
//	// Register the script string type
//	// Look at the implementation for this function for more information  
//	// on how to register a custom string type, and other object types.
//	RegisterStdString(engine);
//
//	if (!strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY"))
//	{
//		// Register the functions that the scripts will be allowed to use.
//		// Note how the return code is validated with an assert(). This helps
//		// us discover where a problem occurs, and doesn't pollute the code
//		// with a lot of if's. If an error occurs in release mode it will
//		// be caught when a script is being built, so it is not necessary
//		// to do the verification here as well.
//		r = engine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION("PrintString"), asCALL_CDECL); assert(r >= 0);
//		r = engine->RegisterGlobalFunction("uint GetSystemTime()", asFUNCTION(timeGetTime), asCALL_STDCALL); assert(r >= 0);
//	}
//	else
//	{
//		// Notice how the registration is almost identical to the above. 
//		r = engine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION("PrintString_Generic"), asCALL_GENERIC); assert(r >= 0);
//		r = engine->RegisterGlobalFunction("uint GetSystemTime()", asFUNCTION("timeGetTime_Generic"), asCALL_GENERIC); assert(r >= 0);
//	}
//
//
//	// It is possible to register the functions, properties, and types in 
//	// configuration groups as well. When compiling the scripts it then
//	// be defined which configuration groups should be available for that
//	// script. If necessary a configuration group can also be removed from
//	// the engine, so that the engine configuration could be changed 
//	// without having to recompile all the scripts.
//}
//
//int ModuleScripting::CompileScript(asIScriptEngine* engine)
//{
//	int r;
//
//	// We will load the script from a file on the disk.
//	FILE* f = fopen("script.as", "rb");
//	if (f == 0)
//	{
//		cout << "Failed to open the script file 'script.as'." << endl;
//		return -1;
//	}
//
//	// Determine the size of the file	
//	fseek(f, 0, SEEK_END);
//	int len = ftell(f);
//	fseek(f, 0, SEEK_SET);
//
//	// On Win32 it is possible to do the following instead
//	// int len = _filelength(_fileno(f));
//
//	// Read the entire file
//	string script;
//	script.resize(len);
//	size_t c = fread(&script[0], len, 1, f);
//	fclose(f);
//
//	if (c == 0)
//	{
//		cout << "Failed to load script file." << endl;
//		return -1;
//	}
//
//	// Add the script sections that will be compiled into executable code.
//	// If we want to combine more than one file into the same script, then 
//	// we can call AddScriptSection() several times for the same module and
//	// the script engine will treat them all as if they were one. The script
//	// section name, will allow us to localize any errors in the script code.
//	asIScriptModule* mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
//	r = mod->AddScriptSection("script", &script[0], len);
//	if (r < 0)
//	{
//		cout << "AddScriptSection() failed" << endl;
//		return -1;
//	}
//
//	// Compile the script. If there are any compiler messages they will
//	// be written to the message stream that we set right after creating the 
//	// script engine. If there are no errors, and no warnings, nothing will
//	// be written to the stream.
//	r = mod->Build();
//	if (r < 0)
//	{
//		cout << "Build() failed" << endl;
//		return -1;
//	}
//
//	// The engine doesn't keep a copy of the script sections after Build() has
//	// returned. So if the script needs to be recompiled, then all the script
//	// sections must be added again.
//
//	// If we want to have several scripts executing at different times but 
//	// that have no direct relation with each other, then we can compile them
//	// into separate script modules. Each module use their own namespace and 
//	// scope, so function names, and global variables will not conflict with
//	// each other.
//
//	return 0;
//}