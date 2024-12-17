#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/Level.h>
#include "TitleGameMode.h"

CreateContentsCoreDefine(UContentsCore);

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	_Data.WindowPos = { 100, 100 };
	_Data.WindowSize = { 1280, 720 };

	// UEngineCore::CreateLevel<ATitleGameMode, APawn>("Titlelevel");
	UEngineCore::CreateLevel<ATitleGameMode, APawn>("Titlelevel");
	UEngineCore::OpenLevel("Titlelevel");
}

void UContentsCore::EngineTick(float _DeltaTime)
{

}

void UContentsCore::EngineEnd()
{

}