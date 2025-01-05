#pragma once
#include <string>

class CGlobal	// Contens global
{
public:
	static const char* ModelPath;
	static std::string GetModelPath(std::string_view _append, std::string_view _fileName);
	static const char* OBJ_SHADER_NAME;
};