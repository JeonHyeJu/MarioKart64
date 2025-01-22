#include "PreCompile.h"
#include "CircuitLoader.h"
#include "CGlobal.h"
#include "GameData.h"
#include <EngineCore/EngineVertex.h>
#include <EngineCore/EngineVertexBuffer.h>
#include <EngineCore/EngineIndexBuffer.h>
#include <EngineCore/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

CircuitLoader::CircuitLoader()
{
	RenderInfos.reserve(10000);
	VertexNavDatas.reserve(10000);
}

CircuitLoader::~CircuitLoader()
{

}

void CircuitLoader::Reset()
{
	RenderInfos.clear();
	VertexNavDatas.clear();
	NavTextures.clear();
}

void CircuitLoader::Load(ECircuit _type, const std::map<std::string, ENavType>& _navTypes, std::string_view _path)
{
	NavTextures = _navTypes;
	MapType = _type;

	std::string path = _path.data();
	ObjPath = path + std::string(".obj");
	MtlPath = path + std::string(".mtl");

	size_t idx = path.find_last_of("/\\");
	FolderName = path.substr(0, idx);
	FileName = path.substr(idx + 1, -1);

	SRenderInfo::MapInfos.insert({ MapType, SMapInfo{} });	// Init

	if (!LoadModel())
	{
		MSGASSERT("Obj파일 로딩에 실패했습니다.");
	}

	SRenderInfo::MapInfos.find(MapType)->second.RenderInfos = RenderInfos;	// Copy
	InitNavMesh();

	Reset();
}

bool CircuitLoader::LoadModel()
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

void CircuitLoader::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
	std::vector<FEngineVertex> vertices;
	std::vector<UINT> indices;

	vertices.reserve(10000);
	indices.reserve(10000);

	float minX = 99999.f, minY = 99999.f, minZ = 99999.f;
	float maxX = 0.f, maxY = 0.f, maxZ = 0.f;
	for (UINT i = 0; i < _mesh->mNumVertices; ++i)
	{
		FEngineVertex vertex;
		vertex.POSITION.X = _mesh->mVertices[i].x;
		vertex.POSITION.Y = _mesh->mVertices[i].y;
		vertex.POSITION.Z = _mesh->mVertices[i].z;
		vertex.POSITION.W = 1.f;

		if (vertex.POSITION.X < minX) minX = vertex.POSITION.X;
		if (vertex.POSITION.Y < minY) minY = vertex.POSITION.Y;
		if (vertex.POSITION.Z < minZ) minZ = vertex.POSITION.Z;
		if (vertex.POSITION.X > maxX) maxX = vertex.POSITION.X;
		if (vertex.POSITION.Y > maxY) maxY = vertex.POSITION.Y;
		if (vertex.POSITION.Z > maxZ) maxZ = vertex.POSITION.Z;

		if (_mesh->mTextureCoords[0])
		{
			vertex.TEXCOORD.X = (float)_mesh->mTextureCoords[0][i].x;
			vertex.TEXCOORD.Y = (float)_mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}
	vertices.resize(vertices.size());

	for (UINT i = 0; i < _mesh->mNumFaces; ++i)
	{
		aiFace face = _mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	indices.resize(indices.size());

	std::string matName = CGlobal::OBJ_SHADER_NAME;
	std::string texName = "NSBase.png";
	/*if (_mesh->mNumVertices == 6)	// temp
	{
		matName = CGlobal::OBJ_SPRITE_SHADER_NAME;
	}*/

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

	// Temp.. TODO: use pos org! and optimization
	// To match mesh index
	if (NavTextures.contains(texName))
	{
		for (size_t i = 0, size = vertices.size() - 3; i < size; i += 3)
		{
			vertices[i].COLOR = float4{ static_cast<float>(DebugIndex), 0.f, 0.f, 1.f };
			vertices[i + 1].COLOR = float4{ static_cast<float>(DebugIndex), 0.f, 0.f, 1.f };
			vertices[i + 2].COLOR = float4{ static_cast<float>(DebugIndex), 0.f, 0.f, 1.f };
			DebugIndex++;
		}
	}

	std::string name = FileName + "_" + std::to_string(MeshCount++);
	UEngineVertexBuffer::Create(name, vertices);
	UEngineIndexBuffer::Create(name, indices);
	UMesh::Create(name);

	SRenderInfo info;
	info.Name = name;
	info.MatName = matName;
	info.TexName = texName;
	info.Z = minZ;

	// Temp
	//if (texName == "7EEAA53A_fix.png" || texName == "922DEA6_c.png" || texName == "3A87458D_c.png" || texName == "5B7CDDF2_fix.png")
	if (NavTextures.contains(texName))
	{
		SVertexToNavData data;
		data.Vertecies = vertices;
		data.FloorType = NavTextures.find(texName)->second;

		if (data.FloorType == ENavType::ROAD)
		{
			SMapSizeInfo& refSize = SRenderInfo::MapInfos.find(MapType)->second.MapSizeInfo;
			if (minX < refSize.Min.X) refSize.Min.X = minX;
			if (minY < refSize.Min.Y) refSize.Min.Y = minY;
			if (minZ < refSize.Min.Z) refSize.Min.Z = minZ;
			if (maxX > refSize.Max.X) refSize.Max.X = maxX;
			if (maxY > refSize.Max.Y) refSize.Max.Y = maxY;
			if (maxZ > refSize.Max.Z) refSize.Max.Z = maxZ;
		}

		VertexNavDatas.push_back(data);
		//RenderInfos.push_back(info);
	}

	// TODO: uncomment
	RenderInfos.push_back(info);
}

void CircuitLoader::ProcessNode(aiNode* node, const aiScene* scene)
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

void CircuitLoader::InitNavMesh()
{
	// TODO: deserialize
	
	std::vector<SNavData>& navData = SRenderInfo::MapInfos.find(MapType)->second.NavInfos;
	navData.reserve(10000);

	int idx = 0;
	for (const SVertexToNavData& vnData : VertexNavDatas)
	{
		const std::vector<FEngineVertex>& vec = vnData.Vertecies;
		for (size_t i = 0, size = vec.size(); i < size; i += 3)
		{
			SNavData nd;
			nd.Vertex[0] = vec[i].POSITION;
			nd.Vertex[1] = vec[i + 1].POSITION;
			nd.Vertex[2] = vec[i + 2].POSITION;
			nd.Index = idx++;
			nd.FloorType = vnData.FloorType;

			navData.push_back(nd);
		}
	}

	int groupIdx = 0;
	for (size_t i = 0, size = navData.size(); i < size - 1; ++i)
	{
		for (size_t j = i + 1; j < size; j++)
		{
			SNavData& leftNd = navData[i];
			SNavData& rightNd = navData[j];
			if (leftNd.GroupIndex == -1)
			{
				leftNd.GroupIndex = groupIdx;
			}

			if (leftNd.IsAttached(rightNd, 1e-1f))
			{
				leftNd.LinkBoth(rightNd);
				if (rightNd.GroupIndex == -1)
				{
					rightNd.GroupIndex = groupIdx;
				}
			}
		}

		groupIdx++;
	}

	// Temp.. Not both
	if (MapType == ECircuit::ROYAL_RACEWAY)
	{
		navData[373].Link(navData[125]);
		navData[373].Link(navData[124]);
		navData[373].Link(navData[123]);
		navData[373].Link(navData[122]);
		navData[373].Link(navData[121]);
		navData[373].Link(navData[120]);
	}

	navData.resize(navData.size());

	VertexNavDatas.clear();
}