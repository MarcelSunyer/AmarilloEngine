#include "Functions.h"

bool TextCmp(const char* text1, const char* text2)
{
	bool ret = false;

	if (text1 == nullptr || text2 == nullptr)
	{
		return false;
	}

	if (strcmp(text1, text2) == 0)
	{
		ret = true;
	}

	return ret;
}

void TextCpy(char* destination, const char* origen)
{
	if (origen != nullptr)
	{
		strcpy_s(destination, strlen(origen), origen);
	}
}

std::string ToUpperCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = toupper(str[i]);
	}

	return str;
}

std::string ToLowerCase(std::string str)
{
	for (uint i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}
