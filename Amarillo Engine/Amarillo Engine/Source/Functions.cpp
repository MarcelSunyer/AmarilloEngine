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
/*
int GetRandomValue(int range_1, int range_2)
{
	 rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(range_1, range_2);

	return dis(gen);
}
int GetUIDRandomInt()
{
	return GetRandomValue(0, INT_MAX);
}

std::string IntToHexa(int value)
{
	std::stringstream stream;
	stream << std::hex << value;
	std::string result(stream.str());
	return result;
}*/

//std::string GetUIDRandomHexadecimal()
//{
//	double val = GetUIDRandomInt();
//	std::string ret = IntToHexa(val);
//
//	return ret;
//}