#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/Level.h>
#include <EngineCore/Mesh.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineIndexBuffer.h>
#include <EngineCore/EngineVertex.h>
#include <EngineCore/EngineVertexBuffer.h>
#include <EngineCore/EngineSprite.h>
#include <EngineCore/EngineShader.h>
#include <EngineCore/EngineMaterial.h>
#include "CGlobal.h"
#include "TitleGameMode.h"
#include "PlayGameMode.h"

#include "TestGameMode.h"	// for test

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

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
	InitImages("Resources\\Models\\Courses\\Royal_Raceway");

	UEngineSprite::CreateSpriteToMeta("Mario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Title_Screen.png", ".meta");

	InitGraphics();
	
	//UEngineCore::CreateLevel<ATitleGameMode, APawn>("TitleLevel");
	//UEngineCore::CreateLevel<ASelectGameMode, APawn>("SelectLevel");
	UEngineCore::CreateLevel<APlayGameMode, APawn>("PlayLevel");

	//UEngineCore::OpenLevel("TitleLevel");
	UEngineCore::OpenLevel("PlayLevel");

	/*UEngineCore::CreateLevel<ATestGameMode, APawn>("TestLevel");
	UEngineCore::OpenLevel("TestLevel");*/
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

	// init materials
	{
		UEngineDirectory dir;
		dir.MoveParentToDirectory("Resources\\Shaders");

		std::vector<UEngineFile> shaderFiles = dir.GetAllFile(true, { ".fx", ".hlsl" });
		for (size_t i = 0; i < shaderFiles.size(); i++)
		{
			UEngineShader::ReflectionCompile(shaderFiles[i]);
		}

		std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_SHADER_NAME);
		mat->SetVertexShader("VertexShader.fx");
		mat->SetPixelShader("PixelShader.fx");
	}
}


