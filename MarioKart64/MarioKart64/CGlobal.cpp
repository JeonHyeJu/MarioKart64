#include "PreCompile.h"
#include "CGlobal.h"
#include <EngineBase/EngineDirectory.h>

const char* CGlobal::ModelPath = "MarioKart64\\Resources\\Models";
const char* CGlobal::OBJ_SHADER_NAME = "Object";
const char* CGlobal::OBJ_SPRITE_SHADER_NAME = "ObjectSprite";
const char* CGlobal::OBJ_SKY_SHADER_NAME = "Skybox";

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