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

private:
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

	std::vector<AiMesh> Meshes;
	std::vector<TEXTURE> Textures;
	std::string Directory = "";
};
