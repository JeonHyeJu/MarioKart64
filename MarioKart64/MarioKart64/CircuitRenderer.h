#pragma once
#include <EngineCore/Renderer.h>
#include <EngineBase/EngineDefine.h>
#include <vector>
#include <assimp/scene.h>
#include "CData.h"

class UEngineCamera;
class ModelLoader;
class CircuitRenderer : public URenderer
{
	friend class UEngineCamera;

public:
	struct RenderInfo
	{
		std::string Name = "";	// Vertex buffer, Pixel buffer, Mesh
		std::string MatName = "";	// Material
		std::string TexName = "";	// Texture
		float Z = 0.f;
	};

	CircuitRenderer();
	~CircuitRenderer();

	CircuitRenderer(const CircuitRenderer& _other) = delete;
	CircuitRenderer(CircuitRenderer&& _other) noexcept = delete;
	CircuitRenderer& operator=(const CircuitRenderer& _other) = delete;
	CircuitRenderer& operator=(CircuitRenderer&& _other) noexcept = delete;

	void Init(std::string_view _path);
	void Init(std::string_view _objPath, std::string_view _mtlPath);

	const std::vector<SNavData>& GetNavData() const
	{
		return NavDatas;
	}
	const SNavData& GetNavData(int _idx) const
	{
		if (_idx < NavDatas.size())
		{
			return NavDatas[_idx];
		}
		else
		{
			return Base;
		}
	}
	const SNavData& GetCurNavData() const
	{
		if (CollidedNavIndex > -1)
		{
			return NavDatas[CollidedNavIndex];
		}
		else
		{
			return Base;
		}
	}
	int GetNavIndex() const
	{
		return CollidedNavIndex;
	}
	void SetNavIndex(int _idx)
	{
		CollidedNavIndex = _idx;
	}

	void SetDebugLocation(const FVector& _loc)
	{
		Debug.Index = _loc;
	}

protected:
	void BeginPlay() override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	void _Init();
	void Sort();
	bool LoadModel();
	void ProcessMesh(struct aiMesh* _mesh, const struct aiScene* _scene);
	void ProcessNode(struct aiNode* node, const struct aiScene* scene);

	void InitNavMesh(const std::vector<SVertexToNavData>& _vec);

	std::string ObjPath = "";
	std::string MtlPath = "";
	std::string Directory = "";
	std::string FileName = "";

	std::vector<RenderInfo> RenderInfos;
	int MeshCount = 0;

	int CollidedNavIndex = -1;
	SNavData Base;
	std::vector<SNavData> NavDatas;
	std::vector<SVertexToNavData> VertexNavDatas;

	FDebug Debug;
	int DebugIndex = 0;
};
