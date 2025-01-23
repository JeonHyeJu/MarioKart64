#pragma once
#include "EngineShaderResources.h"
#include "Mesh.h"
#include "EngineMaterial.h"
#include "EngineEnums.h"

class URenderUnit
{
public:
	URenderUnit();
	~URenderUnit();

	class UTransformObject* TransformObject = nullptr;
	class URenderer* ParentRenderer = nullptr;

	std::shared_ptr<UMesh> Mesh;
	std::shared_ptr<UEngineMaterial> Material;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayOut;

	ENGINEAPI void SetMesh(std::string_view _Name);
	ENGINEAPI void SetMaterial(std::string_view _Name);

	ENGINEAPI virtual void Render(class UEngineCamera* _Camera, float _DeltaTime);

	ENGINEAPI virtual void RenderInst(class UEngineCamera* _Camera, UINT _InstCount, float _DeltaTime);

	ENGINEAPI void MaterialResourcesCheck();

	template<typename Data>
	ENGINEAPI void ConstantBufferLinkData(std::string_view _Name, Data& _Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&_Data));
	}

	ENGINEAPI void ConstantBufferLinkData(std::string_view Name, void* _Data);

	template<typename Data>
	ENGINEAPI void StructuredBufferLinkData(std::string_view _Name, std::vector<Data>& _Data)
	{
		StructuredBufferLinkData(_Name, static_cast<UINT>(_Data.size()), reinterpret_cast<void*>(&_Data[0]));
	}

	ENGINEAPI void StructuredBufferLinkData(std::string_view Name, UINT _Count, void* _Data);

	ENGINEAPI void SetTexture(std::string_view _Name, std::string_view _ResName);
	ENGINEAPI void SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);
	ENGINEAPI void SetSampler(std::string_view Name, std::string_view _ResName);

	ENGINEAPI void SetTexture(std::string_view _Name, UEngineTexture* _Texture);

	ENGINEAPI void Reset();

	std::map<EShaderType, UEngineShaderResources> Resources;

private:
	void InputLayOutCreate();
};

