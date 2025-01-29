#include "PreCompile.h"
#include "CGlobal.h"
#include <EngineBase/EngineDirectory.h>

const char* CGlobal::ModelPath = "MarioKart64\\Resources\\Models";
const char* CGlobal::OBJ_SHADER_NAME = "Object";
const char* CGlobal::OBJ_SPRITE_SHADER_NAME = "ObjectSprite";
const char* CGlobal::OBJ_SKY_SHADER_NAME = "Skybox";
const char* CGlobal::OBJ_LINE_SHADER_NAME = "Line";
const char* CGlobal::WIDGET_SHADER = "WidgetShader";
const char* CGlobal::COLOR_WIDGET_MATERIAL = "ColorWidgetMaterial";
const char* CGlobal::SHRINK_EFFECT = "ShrinkFx";
const char* CGlobal::EXPAND_EFFECT = "ExpandFx";
const int CGlobal::WINDOW_W = 625 * 2;
const int CGlobal::WINDOW_H = 395 * 2;
const float CGlobal::FWINDOW_W = static_cast<float>(625 * 2);
const float CGlobal::FWINDOW_H = static_cast<float>(395 * 2);

std::string CGlobal::GetModelPath(std::string_view _append="", std::string_view _fileName="")
{
	UEngineDirectory dir;
	dir.MoveParentToDirectory(CGlobal::ModelPath);
	if (_append.size() > 0)
	{
		dir.Append(_append.data());
	}

	if (_fileName.size() == 0)
	{
		return dir.GetPathToString() + "\n";
	}

	dir.Append(_fileName.data());
	return dir.GetPathToString();
}