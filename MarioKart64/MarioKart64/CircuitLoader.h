#pragma once
#include <EngineBase/EngineSerializer.h>
#include "CData.h"

class CircuitLoader : public ISerializObject
{
public:
	CircuitLoader();
	~CircuitLoader();

	CircuitLoader(const CircuitLoader& _other) = delete;
	CircuitLoader(CircuitLoader&& _other) noexcept = delete;
	CircuitLoader& operator=(const CircuitLoader& _other) = delete;
	CircuitLoader& operator=(CircuitLoader&& _other) noexcept = delete;

	void Load(ECircuit _type, const std::map<std::string, ENavType>& _navTypes, std::string_view _path);
	void Reset();

private:
	bool LoadModel();
	void ProcessMesh(struct aiMesh* _mesh, const struct aiScene* _scene);
	void ProcessNode(struct aiNode* node, const struct aiScene* scene);
	void InitNavMesh();

	void Serialize(UEngineSerializer& _Ser) override;
	void DeSerialize(UEngineSerializer& _Ser) override;

	std::string FileName = "";
	std::string FolderName = "";
	std::string ObjPath = "";
	std::string MtlPath = "";

	std::vector<SRenderInfo> RenderInfos;
	std::vector<SVertexToNavData> VertexNavDatas;
	std::map<std::string, ENavType> NavTextures;

	ECircuit MapType = ECircuit::END;
	int DebugIndex = 0;
	int MeshCount = 0;
};
