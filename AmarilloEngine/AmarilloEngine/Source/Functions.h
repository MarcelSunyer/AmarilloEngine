#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "Globals.h"
#include <list>
#include <string>
#include "../External/MathGeoLib/include/Math/float2.h"
#include "../External/MathGeoLib/include/Math/float3.h"
#include "../External/MathGeoLib/include/Math/float4.h"
#include "../External/MathGeoLib/include/Math/Rect.h"
#include <random>
#include <limits>
#include "Random.h"

class ImVec2;
class ImVec4;


// USEFULL FUNCTIONS //

// Compares two char*, true if success
__declspec(dllexport) bool TextCmp(const char* text1, const char* text2);

// Copies a string to another
void TextCpy(char* destination, const char* origen);

// Returns chars to upper case
std::string ToUpperCase(std::string str);

// Returns chars to lower case
std::string ToLowerCase(std::string str);

int GetRandomValue(int range_1, int range_2);
int GetUIDRandomInt();

std::string IntToHexa(int value);

std::string GetUIDRandomHexadecimal();


#endif //__FUNCTIONS_H__