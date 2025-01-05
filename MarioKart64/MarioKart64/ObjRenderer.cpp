#include "PreCompile.h"
#include "ObjRenderer.h"
#include "CGlobal.h"
#include <EngineCore/EngineGraphicDevice.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineCore.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void ObjRenderer::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	std::vector<FEngineVertex> vertices;
	std::vector<UINT> indices;

	vertices.reserve(10000);
	indices.reserve(10000);

	for (UINT i = 0; i < _mesh->mNumVertices; ++i)
	{
		FEngineVertex vertex;
		vertex.POSITION.X = _mesh->mVertices[i].x;
		vertex.POSITION.Y = _mesh->mVertices[i].y;
		vertex.POSITION.Z = _mesh->mVertices[i].z;
		vertex.POSITION.W = 1.f;

		if (_mesh->mTextureCoords[0])
		{
			vertex.TEXCOORD.X = (float)_mesh->mTextureCoords[0][i].x;
			vertex.TEXCOORD.Y = (float)_mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < _mesh->mNumFaces; ++i)
	{
		aiFace face = _mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = _scene->mMaterials[_mesh->mMaterialIndex];
		aiString name = mat->GetName();
		UINT size = mat->GetTextureCount(aiTextureType_DIFFUSE);

		if (size == 0)
		{
			OutputDebugStringA(("_mesh->mMaterialIndex: " + std::to_string(_mesh->mMaterialIndex) + "\n").c_str());
			OutputDebugStringA((name.C_Str() + std::string(", size: ") + std::to_string(size) + "\n").c_str());
			Textures.push_back("NSBase.png");
		}
		else
		{
			for (UINT i = 0; i < size; ++i)	// almost size == 1
			{
				aiString str;
				mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
				Textures.push_back(str.C_Str());
			}
		}
	}

	std::string name = FileName + "_" + std::to_string(MeshCount++);
	UEngineVertexBuffer::Create(name, vertices);
	UEngineIndexBuffer::Create(name, indices);
	UMesh::Create(name);
}

void ObjRenderer::ProcessNode(aiNode* node, const aiScene* scene)
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

bool ObjRenderer::LoadModel()
{
	std::string fileName = ObjPath.data();
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

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

ObjRenderer::ObjRenderer()
{
	Textures.reserve(10000);
}

ObjRenderer::~ObjRenderer()
{
}

void ObjRenderer::Init(std::string_view _path)
{
	std::string path = _path.data();
	ObjPath = path + std::string(".obj");
	MtlPath = path + std::string(".mtl");

	size_t idx = path.find_last_of("/\\");
	Directory = path.substr(0, idx);
	FileName = path.substr(idx+1, -1);

	_Init();
}

void ObjRenderer::_Init()
{
	if (!LoadModel())
	{
		OutputDebugStringA(("[WARN] " + ObjPath + " is not set.").c_str());
	}

	for (int i = 0; i < MeshCount; ++i)
	{
		std::string name = FileName + "_" + std::to_string(i);

		URenderUnit& unit = CreateRenderUnit();
		SetMesh(name, i);
		SetMaterial(CGlobal::OBJ_SHADER_NAME, i);
		unit.SetTexture("diffTexture", Textures[i]);
	}
}

void ObjRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();	// Don't use URenderer::BeginPlay()
	SetOrder(0);
}

void ObjRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	/*if (nullptr != CurAnimation)
	{
		Sprite = CurAnimation->Sprite;

		GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(CurIndex)->GetName());
		SpriteData = Sprite->GetSpriteData(CurIndex);
	}

	if (true == IsAutoScale)
	{
		FVector Scale = Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}*/

	URenderer::Render(_camera, _deltaTime);
}
