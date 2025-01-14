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
#include <EngineCore/HUD.h>
#include "CGlobal.h"
#include "TitleGameMode.h"
#include "PlayGameMode.h"

#include "TestGameMode.h"	// for test
#include "TestVertex.h"	// for test

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

void UContentsCore::InitTextures(std::string_view _path)
{
	UEngineDirectory dir;
	if (false == dir.MoveParentToDirectory(_path.data()))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	std::vector<UEngineFile> ImageFiles = dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UEngineTexture::Load(FilePath);
	}
}

void UContentsCore::InitSprites(std::string_view _path)
{
	UEngineDirectory dir;
	if (false == dir.MoveParentToDirectory(_path.data()))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	UEngineSprite::CreateSpriteToFolder(dir.GetPathToString());
}

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	_Data.WindowPos = { 100, 100 };
	_Data.WindowSize = { 1280, 720 };

	InitTextures("Resources\\Sprites\\Characters");
	InitTextures("Resources\\Sprites\\Background");
	InitTextures("Resources\\Sprites\\Title");
	InitTextures("Resources\\Sprites\\Clouds");
	InitTextures("Resources\\Sprites\\GameObjects");
	InitTextures("Resources\\Models\\Courses\\Royal_Raceway");

	InitSprites("Resources\\Sprites\\Background");
	UEngineSprite::CreateSpriteToMeta("Mario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Title_Screen.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Items.png", ".meta");

	InitGraphics();
	InitTest();
	
	//UEngineCore::CreateLevel<ATitleGameMode, APawn, AHUD>("TitleLevel");
	//UEngineCore::CreateLevel<ASelectGameMode, APawn, AHUD>("SelectLevel");
	UEngineCore::CreateLevel<APlayGameMode, APawn, AHUD>("PlayLevel");

	//UEngineCore::OpenLevel("TitleLevel");
	UEngineCore::OpenLevel("PlayLevel");

	/*UEngineCore::CreateLevel<ATestGameMode, APawn, AHUD>("TestLevel");
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
	// Rasterizer state
	{
		D3D11_RASTERIZER_DESC desc = {};
		// Temp. for test
		//desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		//desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		UEngineRasterizerState::Create("CullBack", desc);
	}

	// init materials
	{
		// Shaders
		UEngineDirectory dir;
		dir.MoveParentToDirectory("Resources\\Shaders");

		std::vector<UEngineFile> shaderFiles = dir.GetAllFile(true, { ".fx", ".hlsl" });
		for (size_t i = 0; i < shaderFiles.size(); i++)
		{
			UEngineShader::ReflectionCompile(shaderFiles[i]);
		}

		std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_SHADER_NAME);
		std::shared_ptr<UEngineMaterial> matSprite = UEngineMaterial::Create(CGlobal::OBJ_SPRITE_SHADER_NAME);
		std::shared_ptr<UEngineMaterial> matSky = UEngineMaterial::Create(CGlobal::OBJ_SKY_SHADER_NAME);
		std::shared_ptr<UEngineMaterial> matLine = UEngineMaterial::Create(CGlobal::OBJ_LINE_SHADER_NAME);

		mat->SetRasterizerState("CullBack");
		mat->SetVertexShader("VertexShader.fx");
		mat->SetPixelShader("PixelShader.fx");

		matSprite->SetVertexShader("VertexShader.fx");
		matSprite->SetPixelShader("PixelShader.fx");

		matSky->SetVertexShader("ColorShader.fx");
		matSky->SetPixelShader("ColorShader.fx");
		matSky->SetRasterizerState("CullBack");	// Temp
		
		matLine->SetVertexShader("ColorShader.fx");
		matLine->SetPixelShader("ColorShader.fx");
		matLine->SetTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}

void UContentsCore::InitTest()
{
	// Triangle
	{
		std::vector<UINT> indices;

		indices.reserve(3);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		UEngineVertexBuffer::Create("Triangle", SingleTriangle);
		UEngineIndexBuffer::Create("Triangle", indices);
		UMesh::Create("Triangle");
	}

	// Line
	{
		std::vector<UINT> indices;

		indices.reserve(2);

		indices.push_back(0);
		indices.push_back(1);

		UEngineVertexBuffer::Create("Line", SingleLine);
		UEngineIndexBuffer::Create("Line", indices);
		UMesh::Create("Line");
	}

	// Multiple triangles
	{
		// Temp.. no reserve or resize
		std::vector<UINT> indices;
		
		for (int i = 0; i < MultipleTriangles.size(); ++i)
		{
			indices.push_back(i);
		}

		UEngineVertexBuffer::Create("MultiTriangle", MultipleTriangles);
		UEngineIndexBuffer::Create("MultiTriangle", indices);
		UMesh::Create("MultiTriangle");
	}

	// Cube
	{
		std::vector<UINT> indices = { 2, 1, 0, 2, 3, 1, 5, 4, 0, 0, 1, 5, 2, 6, 7, 7, 3, 2, 0, 4, 2, 4, 6, 2, 3, 5, 1, 3, 7, 5, 4, 7, 6, 4, 5, 7 };

		UEngineVertexBuffer::Create("Cube", SingleCube);
		UEngineIndexBuffer::Create("Cube", indices);
		UMesh::Create("Cube");
	}
}