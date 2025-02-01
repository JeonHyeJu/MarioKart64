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
#include <EngineCore/EngineFont.h>
#include "CGlobal.h"
#include "CircuitLoader.h"
#include "TitleGameMode.h"
#include "SelectGameMode.h"
#include "PlayGameMode.h"
#include "GameData.h"
#include "UIPlay.h"
#include "Player.h"
#include "Driver.h"

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
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	float horizontal = static_cast<float>(desktop.right);
	float vertical = static_cast<float>(desktop.bottom);

	_Data.WindowPos = { (horizontal - CGlobal::WINDOW_W) * .5f, (vertical - CGlobal::WINDOW_H) * .5f };
	_Data.WindowSize = { CGlobal::WINDOW_W, CGlobal::WINDOW_H };

	InitTextures("Resources\\Sprites\\Characters");
	InitTextures("Resources\\Sprites\\Background");
	InitTextures("Resources\\Sprites\\Title");
	InitTextures("Resources\\Sprites\\Clouds");
	InitTextures("Resources\\Sprites\\GameObjects");
	InitTextures("Resources\\Sprites\\SelectGame");
	InitTextures("Resources\\Sprites\\SelectCharacter");
	InitTextures("Resources\\Sprites\\TrackIcons");
	InitTextures("Resources\\Sprites\\UI");
	InitTextures("Resources\\Sprites\\LabCount");
	InitTextures("Resources\\Models\\Courses\\Royal_Raceway");
	InitTextures("Resources\\Models\\Courses\\Luigi_Raceway");
	InitTextures("Resources\\Models\\Courses\\Koopa_Troopa_Beach");
	InitTextures("Resources\\Models\\Courses\\Mario_Raceway");
	InitTextures("Resources\\Models\\Courses\\Wario_Stadium");
	InitTextures("Resources\\Models\\Courses\\Sherbet_Land");
	InitTextures("Resources\\Models\\Courses\\Bowsers_Castle");
	InitTextures("Resources\\Models\\Courses\\Rainbow_Road");
	InitTextures("Resources\\Models\\Miscellaneous\\Nintendo_Logo");
	InitTextures("Resources\\Models\\Miscellaneous\\Title_Screen_Flag");

	InitSprites("Resources\\Sprites\\Background");
	InitSprites("Resources\\Sprites\\SelectGame");
	InitSprites("Resources\\Sprites\\SelectCharacter");
	InitSprites("Resources\\Sprites\\LabCount");

	UEngineSprite::CreateSpriteToMeta("Mario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Luigi.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Peach.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Yoshi.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Wario.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Bowser.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Title_Screen.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Items.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("SelectCharacter.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("TrackIcons.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("FontAndPositions.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Timer.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("RacePositionIcons.png", ".meta");
	UEngineSprite::CreateSpriteToMeta("Lakitu.png", ".meta");

	InitGraphics();
	InitTest();
	InitObjs();

	UEngineFont::Load("Arial", "Arial");

	UEngineCore::CreateLevel<ATitleGameMode, APawn, AHUD>("TitleLevel");
	UEngineCore::CreateLevel<ASelectGameMode, APawn, AHUD>("SelectLevel");
	UEngineCore::CreateLevel<APlayGameMode, APlayer, AUIPlay>("PlayLevel");
	//UEngineCore::CreateLevel<APlayGameMode, ADriver, AUIPlay>("PlayLevel");

	//UEngineCore::OpenLevel("TitleLevel");
	//UEngineCore::OpenLevel("SelectLevel");
	UEngineCore::OpenLevel("PlayLevel");

	/*UEngineCore::CreateLevel<ATestGameMode, APawn, AHUD>("TestLevel");
	UEngineCore::OpenLevel("TestLevel");*/

	// for test
	std::vector<SPlayerInfo> palyerInfos;
	for (int i = 0; i < 6; ++i)
	{
		palyerInfos.emplace_back(SPlayerInfo{ static_cast<ECharacter>(i), EItemType::NONE });
	}
	GameData::GetInstance()->SetPlayerIdx(2);
	GameData::GetInstance()->SetPlayers(palyerInfos);

	SMapPackage mapPackage;
	//mapPackage.Maps[0] = ECircuit::WARIO_STADIUM;
	mapPackage.Maps[0] = ECircuit::LUIGI_RACEWAY;
	mapPackage.Maps[1] = ECircuit::ROYAL_RACEWAY;
	mapPackage.Maps[2] = ECircuit::MARIO_RACEWAY;
	mapPackage.Maps[3] = ECircuit::WARIO_STADIUM;
	GameData::GetInstance()->SetMapPakcage(mapPackage);

	/*mapPackage.Maps[0] = ECircuit::SHERBET_LAND;
	mapPackage.Maps[1] = ECircuit::KOOPA_TROOPA_BEACH;
	mapPackage.Maps[2] = ECircuit::BOWSERS_CASTLE;
	mapPackage.Maps[3] = ECircuit::RAINBOW_ROAD;
	GameData::GetInstance()->SetMapPakcage(mapPackage);*/
}

void UContentsCore::EngineTick(float _DeltaTime)
{
}

void UContentsCore::EngineEnd()
{
	GameData::GetInstance()->Release();
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

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_SHADER_NAME);
			mat->SetRasterizerState("CullBack");
			mat->SetVertexShader("VertexShader.fx");
			mat->SetPixelShader("PixelShader.fx");
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_SPRITE_SHADER_NAME);
			mat->SetVertexShader("VertexShader.fx");
			mat->SetPixelShader("PixelShader.fx");
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_SKY_SHADER_NAME);
			mat->SetVertexShader("ColorShader.fx");
			mat->SetPixelShader("ColorShader.fx");
			//mat->SetRasterizerState("CullBack");
		}
		
		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::OBJ_LINE_SHADER_NAME);
			mat->SetVertexShader("ColorShader.fx");
			mat->SetPixelShader("ColorShader.fx");
			mat->SetTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::SHRINK_EFFECT);
			mat->SetVertexShader("ShrinkEffect.fx");
			mat->SetPixelShader("ShrinkEffect.fx");
			mat->SetDepthStencilState("ALWAYS");
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::EXPAND_EFFECT);
			mat->SetVertexShader("ExpandEffect.fx");
			mat->SetPixelShader("ExpandEffect.fx");
			mat->SetDepthStencilState("ALWAYS");
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::WIDGET_SHADER);
			mat->SetVertexShader("WidgetShader.fx");
			mat->SetPixelShader("WidgetShader.fx");
			mat->SetDepthStencilState("UIDepth");
		}

		{
			std::shared_ptr<UEngineMaterial> mat = UEngineMaterial::Create(CGlobal::COLOR_WIDGET_MATERIAL);
			mat->SetVertexShader("LetterboxShader.fx");
			mat->SetPixelShader("LetterboxShader.fx");
			mat->SetDepthStencilState("UIDepth");
		}
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

	// Text
	/*
	{
		const int RECT_SIZE = 4;
		const int MAX_TXT_CNT = 2;
		const int SIZE = MAX_TXT_CNT * RECT_SIZE;
		const float UV_SIZE = 1.f / MAX_TXT_CNT;

		std::vector<FEngineVertex> rect =
		{
			FEngineVertex{ FVector(0.f, 1.f, 0.0f), { 0.0f, 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
			FEngineVertex{ FVector(1.f, 1.f, 0.0f), { UV_SIZE, 0.f }, {1.0f, 0.0f, 0.0f, 1.0f} },
			FEngineVertex{ FVector(0.f, 0.f, 0.0f), { 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
			FEngineVertex{ FVector(1.f, 0.f, 0.0f), { UV_SIZE, 1.f }, {1.0f, 0.0f, 0.0f, 1.0f} }
		};

		
		std::vector<UINT> indices;
		std::vector<FEngineVertex> textMesh;
		textMesh.reserve(SIZE);
		indices.reserve(MAX_TXT_CNT * 6);

		for (int i = 0; i < RECT_SIZE; ++i)
		{
			textMesh.emplace_back(rect[i]);
		}

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(1);
		indices.emplace_back(3);
		indices.emplace_back(2);

		int last1Idx = 1;
		int last2Idx = 3;

		for (int i = 1; i < MAX_TXT_CNT; ++i)
		{
			float fi = static_cast<float>(i);

			// first triangle
			indices.emplace_back(last1Idx);

			FEngineVertex vtx1 = textMesh[last1Idx];
			vtx1.POSITION += FVector{ 1.f, 0.f, 0.f };
			vtx1.TEXCOORD += FVector{ UV_SIZE, 0.f };
			textMesh.emplace_back(vtx1);

			int _idx = static_cast<int>(textMesh.size() - 1);
			indices.emplace_back(_idx);
			last1Idx = _idx;

			indices.emplace_back(last2Idx);

			// second triangle
			indices.emplace_back(last1Idx);

			FEngineVertex vtx2 = textMesh[last2Idx];
			vtx2.POSITION += FVector{ 1.f, 0.f, 0.f };
			vtx2.TEXCOORD += FVector{ UV_SIZE, 0.f };
			textMesh.emplace_back(vtx2);
			
			_idx = static_cast<int>(textMesh.size() - 1);
			indices.emplace_back(_idx);

			indices.emplace_back(last2Idx);

			last2Idx = _idx;
		}
		
		UEngineVertexBuffer::Create("Text", textMesh);
		UEngineIndexBuffer::Create("Text", indices);
		UMesh::Create("Text");
	}
	*/
}

void UContentsCore::InitObjs()
{
	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "Shape.023.png", ENavType::ROAD });
		navTextures.insert({ "Shape.022.png", ENavType::ROAD });
		navTextures.insert({ "Shape.066.png", ENavType::START_POINT });
		navTextures.insert({ "Shape.182.png", ENavType::BORDER });
		navTextures.insert({ "Shape.007.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Luigi_Raceway", "luigi");
		loader.Load(ECircuit::LUIGI_RACEWAY, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "1E6BB2A4_c.png", ENavType::ROAD });
		navTextures.insert({ "53EFB838_c.png", ENavType::START_POINT });
		navTextures.insert({ "43EFF121_c.png", ENavType::BORDER });
		//navTextures.insert({ "236F857A_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Koopa_Troopa_Beach", "KoopaTroopaBeach");
		loader.Load(ECircuit::KOOPA_TROOPA_BEACH, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "7EEA0D6A_c.png", ENavType::ROAD });
		navTextures.insert({ "92276F6_c.png", ENavType::START_POINT });
		navTextures.insert({ "73879E4_c.png", ENavType::BORDER });
		navTextures.insert({ "3ED14A28_c.png", ENavType::BORDER });
		navTextures.insert({ "6B0CD968_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Mario_Raceway", "Mario_Raceway");
		loader.Load(ECircuit::MARIO_RACEWAY, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "7FE0379A_c.png", ENavType::ROAD });
		navTextures.insert({ "19CDB1E0_c.png", ENavType::START_POINT });
		//navTextures.insert({ "73879E4_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Wario_Stadium", "wario_stadium");
		loader.Load(ECircuit::WARIO_STADIUM, navTextures, path);
	}
	
	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "706CCEED_c.png", ENavType::ROAD });
		navTextures.insert({ "4DD5B9DA_c.png", ENavType::ROAD });
		navTextures.insert({ "71125E3C_c.png", ENavType::ROAD });
		navTextures.insert({ "22F0D5A0_c.png", ENavType::START_POINT });
		//navTextures.insert({ "73879E4_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Sherbet_Land", "SherbetLand");
		loader.Load(ECircuit::SHERBET_LAND, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "7EEAA53A_fix.png", ENavType::ROAD });
		navTextures.insert({ "922DEA6_c.png", ENavType::START_POINT });
		navTextures.insert({ "3A87458D_c.png", ENavType::BORDER });
		navTextures.insert({ "5B7CDDF2_fix.png", ENavType::FLATE_FASTER });

		std::string path = CGlobal::GetModelPath("Courses\\Royal_Raceway", "Royal_Raceway");
		loader.Load(ECircuit::ROYAL_RACEWAY, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "5BC0F9AF_c.png", ENavType::ROAD });
		navTextures.insert({ "61E8F148_c.png", ENavType::ROAD });
		navTextures.insert({ "6A18B68A_c.png", ENavType::ROAD });
		navTextures.insert({ "58291365_c.png", ENavType::ROAD });
		navTextures.insert({ "315D18F3_c.png", ENavType::ROAD });
		navTextures.insert({ "1FBCBE21_c.png", ENavType::ROAD });
		navTextures.insert({ "3A15AC1F_c.png", ENavType::START_POINT });
		navTextures.insert({ "1897EC7_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Bowsers_Castle", "BowsersCastle");
		loader.Load(ECircuit::BOWSERS_CASTLE, navTextures, path);
	}

	{
		CircuitLoader loader;

		std::map<std::string, ENavType> navTextures;
		navTextures.insert({ "448F972_c.png", ENavType::ROAD });
		navTextures.insert({ "19CF4260_c.png", ENavType::START_POINT });
		//navTextures.insert({ "1897EC7_c.png", ENavType::BORDER });

		std::string path = CGlobal::GetModelPath("Courses\\Rainbow_Road", "rainbow");
		loader.Load(ECircuit::RAINBOW_ROAD, navTextures, path);
	}
}