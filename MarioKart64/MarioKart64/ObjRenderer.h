#pragma once
#include <EngineCore/Renderer.h>
#include <EngineBase/EngineDefine.h>
#include <vector>
#include <assimp/scene.h>
#include "AiMesh.h"

class UEngineCamera;
class ModelLoader;
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
	void Init(std::string_view _objPath, std::string_view _mtlPath);

protected:
	void BeginPlay() override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	void _Init();
	void Sort();
	bool LoadModel();
	void ProcessMesh(struct aiMesh* _mesh, const struct aiScene* _scene);
	void ProcessNode(struct aiNode* node, const struct aiScene* scene);

	/*bool LoadModel(std::string_view _objPath, std::string_view _mtlPath);
	virtual void Render(UEngineCamera* _Camera, float _DeltaTime);

	void ProcessNode(aiNode* node, const aiScene* scene);
	AiMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TEXTURE> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

	void InitShader();
	void RasterizerInit() override;
	void ShaderResInit() override;
	void ShaderResSetting() override;*/

	std::string ObjPath = "";
	std::string MtlPath = "";
	std::string Directory = "";
	std::string FileName = "";

	std::vector<RenderInfo> RenderInfos;
	int MeshCount = 0;
};
