#pragma once
#include <EngineCore/Renderer.h>
#include <EngineBase/EngineDefine.h>
#include <vector>
#include <assimp/scene.h>

class UEngineCamera;
class ObjRenderer : public URenderer
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

	ObjRenderer();
	~ObjRenderer();

	ObjRenderer(const ObjRenderer& _other) = delete;
	ObjRenderer(ObjRenderer&& _other) noexcept = delete;
	ObjRenderer& operator=(const ObjRenderer& _other) = delete;
	ObjRenderer& operator=(ObjRenderer&& _other) noexcept = delete;

	void Init(std::string_view _path);

protected:
	void BeginPlay() override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	void _Init();
	void Sort();
	bool LoadModel();
	void ProcessMesh(struct aiMesh* _mesh, const struct aiScene* _scene);
	void ProcessNode(struct aiNode* node, const struct aiScene* scene);

	std::string ObjPath = "";
	std::string MtlPath = "";
	std::string Directory = "";
	std::string FileName = "";

	std::vector<RenderInfo> RenderInfos;
	int MeshCount = 0;
};
