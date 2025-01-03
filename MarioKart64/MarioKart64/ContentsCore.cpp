#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include <EngineCore/EngineShader.h>
#include <EngineCore/EngineMaterial.h>
#include "TitleGameMode.h"
#include "TestGameMode.h"
#include "PlayGameMode.h"

CreateContentsCoreDefine(UContentsCore);

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}

void UContentsCore::InitImages(std::string_view _path)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory(_path.data()))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UEngineTexture::Load(FilePath);
	}
}

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	_Data.WindowPos = { 100, 100 };
	_Data.WindowSize = { 1280, 720 };

	InitImages("Resources\\Sprites\\Characters");
	InitImages("Resources\\Sprites\\Title");

	UEngineSprite::CreateSpriteToMeta("Mario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Title_Screen.png", ".meta");

	InitGraphics();
	
	UEngineCore::CreateLevel<ATestGameMode, APawn>("TestLevel");
	UEngineCore::CreateLevel<ATitleGameMode, APawn>("TitleLevel");
	//UEngineCore::CreateLevel<ASelectGameMode, APawn>("SelectLevel");
	UEngineCore::CreateLevel<APlayGameMode, APawn>("PlayLevel");

	UEngineCore::OpenLevel("TestLevel");
	//UEngineCore::OpenLevel("TitleLevel");
	//UEngineCore::OpenLevel("PlayLevel");
}

void UContentsCore::EngineTick(float _DeltaTime)
{

}

void UContentsCore::EngineEnd()
{

}

/* Graphics */
void UContentsCore::InitGraphics()
{
	// for test
	/*UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("MarioKart64");

	std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}

	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
	Mat->SetVertexShader("SpriteShader.fx");
	Mat->SetPixelShader("SpriteShader.fx");*/

	UEngineDirectory dir;
	dir.MoveParentToDirectory("Resources\\Shaders");

	std::vector<UEngineFile> shaderFiles = dir.GetAllFile(true, { ".fx", ".hlsl" });
	for (size_t i = 0; i < shaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(shaderFiles[i]);
	}

	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
	Mat->SetVertexShader("TestShader.fx");
	Mat->SetPixelShader("TestShader.fx");
}