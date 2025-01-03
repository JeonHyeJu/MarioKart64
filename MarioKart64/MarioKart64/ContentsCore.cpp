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
#include "TitleGameMode.h"
#include "PlayGameMode.h"

#include "TestGameMode.h"	// for test
#include "CGlobal.h"

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
	UEngineDirectory dir;
	dir.MoveParentToDirectory("Resources\\Shaders");

	std::vector<UEngineFile> shaderFiles = dir.GetAllFile(true, { ".fx", ".hlsl" });
	for (size_t i = 0; i < shaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(shaderFiles[i]);
	}

	const std::string objName = "Royal_Raceway";
	std::shared_ptr<UEngineMaterial> material = UEngineMaterial::Create(objName);

	material->SetVertexShader("VertexShader.fx");
	material->SetPixelShader("PixelShader.fx");

	// init meshes
	LoadModel();	// for test
}

// Temp
void ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<FEngineVertex> vertices;
	std::vector<UINT> indices;
	std::vector<UEngineTexture> textures;

	vertices.reserve(10000);
	indices.reserve(10000);
	//textures.reserve(10);

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		FEngineVertex vertex;

		vertex.POSITION = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z,
			1.0f
		};

		if (mesh->mTextureCoords[0])
		{
			vertex.TEXCOORD.X = (float)mesh->mTextureCoords[0][i].x;
			vertex.TEXCOORD.Y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//	std::vector<UEngineTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//}

	UEngineVertexBuffer::Create("Royal_Raceway", vertices);
	UEngineIndexBuffer::Create("Royal_Raceway", indices);
	UMesh::Create("Royal_Raceway");
}

void ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

bool UContentsCore::LoadModel()
{
	std::string path = CGlobal::GetModelPath("Courses\\Royal_Raceway", "Royal_Raceway");
	std::string _objPath = path + ".obj";
	std::string _mtlPath = path + ".mtl";

	std::string fileName = _objPath.data();
	Assimp::Importer importer;

	unsigned int flag;
	flag = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
	//flag = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals;
	//flag = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder;

	const aiScene* pScene = importer.ReadFile(fileName, flag);

	if (pScene == nullptr)
	{
		return false;
	}

	Directory = fileName.substr(0, fileName.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

