#pragma once
#include "SceneComponent.h"
#include <EngineBase/EngineDefine.h>
#include <vector>
#include <assimp/material.h>
#include "Mesh.h"

class UEngineCamera;
class ModelLoader;
class ObjRenderer : public USceneComponent
{
	friend class UEngineCamera;

public:
	ObjRenderer();
	~ObjRenderer();

	ObjRenderer(const ObjRenderer& _other) = delete;
	ObjRenderer(ObjRenderer&& _other) noexcept = delete;
	ObjRenderer& operator=(const ObjRenderer& _other) = delete;
	ObjRenderer& operator=(ObjRenderer&& _other) noexcept = delete;

	bool LoadModel(std::string_view _objPath, std::string_view _mtlPath);

protected:
	ENGINEAPI void BeginPlay() override;

private:
	//virtual void Render(UEngineCamera* _Camera, float _DeltaTime);

	void InitInputAssembler1();
	void InitVertexShader();
	void InitInputAssembler2();
	void InitRasterizer();
	void InitPixelShader();
	void InitOutputMerger();

	void SetInputAssembler1();
	void SetVertexShader();
	void SetInputAssembler2();
	void SetRasterizer();
	void SetPixelShader();
	void SetOutputMerger();

	void ProcessNode(class aiNode* node, const class aiScene* scene);
	Mesh ProcessMesh(class aiMesh* mesh, const aiScene* scene);
	std::vector<AiTexture> LoadMaterialTextures(class aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	class ID3D11ShaderResourceView* LoadEmbeddedTexture(const class aiTexture* embeddedTexture);

	std::string ObjPath = "";
	std::string MtlPath = "";

	std::vector<Mesh> Meshes;
	std::vector<AiTexture> Textures;
	std::string Directory = "";
};
