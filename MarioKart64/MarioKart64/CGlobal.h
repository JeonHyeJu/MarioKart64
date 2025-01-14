#pragma once
#include <string>

class CGlobal	// Contens global
{
public:
	static const char* ModelPath;
	static std::string GetModelPath(std::string_view _append, std::string_view _fileName);
	static const char* OBJ_SHADER_NAME;
	static const char* OBJ_SPRITE_SHADER_NAME;
	static const char* OBJ_SKY_SHADER_NAME;
	static const char* OBJ_LINE_SHADER_NAME;
	static const int ALL_ITEM_SIZE;
};