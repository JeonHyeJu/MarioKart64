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

	float minZ = 999999.f;
	for (UINT i = 0; i < _mesh->mNumVertices; ++i)
	{
		FEngineVertex vertex;
		vertex.POSITION.X = _mesh->mVertices[i].x;
		vertex.POSITION.Y = _mesh->mVertices[i].y;
		vertex.POSITION.Z = _mesh->mVertices[i].z;
		vertex.POSITION.W = 1.f;

		if (vertex.POSITION.Z < minZ)
		{
			minZ = vertex.POSITION.Z;
		}

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

	std::string matName = CGlobal::OBJ_SHADER_NAME;
	std::string texName = "NSBase.png";
	if (_mesh->mNumVertices == 6)	// temp
	{
		matName = CGlobal::OBJ_SPRITE_SHADER_NAME;
	}

	if (_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = _scene->mMaterials[_mesh->mMaterialIndex];
		std::string name = std::string(mat->GetName().C_Str());

		UINT size = mat->GetTextureCount(aiTextureType_DIFFUSE);
		if (size == 0)
		{
			OutputDebugStringA(("_mesh->mMaterialIndex: " + std::to_string(_mesh->mMaterialIndex) + "\n").c_str());
			OutputDebugStringA((name + std::string(", size: ") + std::to_string(size) + "\n").c_str());
		}
		else
		{
			for (UINT i = 0; i < size; ++i)	// almost size == 1
			{
				aiString str;
				mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
				texName = str.C_Str();
			}
		}
	}

	std::string name = FileName + "_" + std::to_string(MeshCount++);
	UEngineVertexBuffer::Create(name, vertices);
	UEngineIndexBuffer::Create(name, indices);
	UMesh::Create(name);

	RenderInfo info;
	info.Name = name;
	info.MatName = matName;
	info.TexName = texName;
	info.Z = minZ;

	RenderInfos.push_back(info);
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
	RenderInfos.reserve(10000);
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

	//Sort();

	int size = static_cast<int>(RenderInfos.size());
	for (int i = 0; i < size; ++i)
	{
		const RenderInfo& unitInfo = RenderInfos[i];
		URenderUnit& unit = CreateRenderUnit();
		SetMesh(unitInfo.Name, i);
		SetMaterial(unitInfo.MatName, i);
		unit.SetTexture("diffTexture", unitInfo.TexName);
	}
}

void ObjRenderer::Sort()
{
	std::sort(RenderInfos.begin(), RenderInfos.end(), [](const RenderInfo& _left, const RenderInfo& _right)
	{
		return _left.Z < _right.Z;
	});
}

void ObjRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();	// Don't use URenderer::BeginPlay()
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
