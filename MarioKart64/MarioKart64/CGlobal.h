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
	static const char* WIDGET_SHADER;
	static const char* SHRINK_EFFECT;
	static const char* EXPAND_EFFECT;
	static const int ALL_ITEM_SIZE;
	static const int WINDOW_W;
	static const int WINDOW_H;
	static const float FWINDOW_W;
	static const float FWINDOW_H;
};