#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <map>

#include "../External/Glew/include/glew.h"
#include <gl/GL.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
__declspec(dllexport) void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846264338327950288

#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------

#ifdef _STANDALONE
#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440
#else
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#endif // _STANDALONE

#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_MAXIMIZED false
#define WIN_BORDERLESS false

#ifdef _STANDALONE
#define WIN_FULLSCREEN_DESKTOP true
#else
#define WIN_FULLSCREEN_DESKTOP false
#endif // _STANDALONE

#define VSYNC true
#define TITLE "Ymir Engine"

// Physics
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Clear a vector
template <class T>
void ClearVec(std::vector<T>& x)
{
	x.clear();
	x.shrink_to_fit();
}

// Clear a vector of pointers
template <class T>
void ClearVecPtr(std::vector<T*>& x)
{
	for (auto it = x.rbegin(); it != x.rend(); ++it)
	{
		// code
		delete(*it);
		(*it) = nullptr;
	}

	x.clear();
	x.shrink_to_fit();

}
//Scripting fuction: Va a buscar un proyecto .sln y lo compila -> El proyecto Assembly-CSharp.sln
static void CMDCompileCS() 
{
    SHELLEXECUTEINFO execInfo{};
    execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    execInfo.hwnd = NULL;
    execInfo.lpVerb = "open"; // Explicitly set the verb to "open"
    execInfo.lpFile = "cmd.exe";
    execInfo.lpParameters = "/C cd mono-runtime\\MSBuild & msbuild ..\\..\\Assembly-CSharp.sln /p:Configuration=Release";
    execInfo.lpDirectory = NULL;
    execInfo.nShow = SW_SHOW; /*SW_SHOW or SW_HIDE*/
    execInfo.hInstApp = NULL;

    if (execInfo.hProcess != NULL) {
        WaitForSingleObject(execInfo.hProcess, INFINITE);
        CloseHandle(execInfo.hProcess);
    }
}
