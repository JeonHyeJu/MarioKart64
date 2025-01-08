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

void UContentsCore::InitImages(std::string_view _path)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory(_path.data()))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
	InitImages("Resources\\Sprites\\Background");
	InitImages("Resources\\Sprites\\Title");
	InitImages("Resources\\Sprites\\Clouds");
	InitImages("Resources\\Models\\Courses\\Royal_Raceway");

	UEngineSprite::CreateSpriteToMeta("Mario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Title_Screen.png", ".meta");

	InitGraphics();
	InitTest();
	
	//UEngineCore::CreateLevel<ATitleGameMode, APawn>("TitleLevel");
	//UEngineCore::CreateLevel<ASelectGameMode, APawn>("SelectLevel");
	//UEngineCore::CreateLevel<APlayGameMode, APawn>("PlayLevel");

	//UEngineCore::OpenLevel("TitleLevel");
	//UEngineCore::OpenLevel("PlayLevel");

	UEngineCore::CreateLevel<ATestGameMode, APawn>("TestLevel");
	UEngineCore::OpenLevel("TestLevel");
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
		desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;	// Temp. for test
		//desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		//desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
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
}