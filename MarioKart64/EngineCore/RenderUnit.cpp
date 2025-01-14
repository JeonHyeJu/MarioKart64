#include "PreCompile.h"
#include "RenderUnit.h"
#include "EngineEnums.h"
#include "Renderer.h"

URenderUnit::URenderUnit()
{
}

URenderUnit::~URenderUnit()
{
}

void URenderUnit::MaterialResourcesCheck()
{
	if (nullptr == Material)
	{
		MSGASSERT("존재하지 않는 머티리얼의 리소스를 체크할 수 없습니다.");
		return;
	}

	{
		UEngineShaderResources& Vs = Material->GetVertexShader()->ShaderResources;
		Resources[EShaderType::VS] = Material->GetVertexShader()->ShaderResources;
	}

	{
		UEngineShaderResources& Ps = Material->GetPixelShader()->ShaderResources;
		Resources[EShaderType::PS] = Material->GetPixelShader()->ShaderResources;
	}

	if (nullptr != ParentRenderer)
	{
		TransformObject = ParentRenderer;
	}

	if (nullptr != TransformObject)
	{
		for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
		{
			if (false == Resources.contains(i))
			{
				continue;
			}

			if (false == Resources[i].IsConstantBuffer("FTransform"))
			{
				continue;
			}

			FTransform& Ref = TransformObject->GetTransformRef();
			Resources[i].ConstantBufferLinkData("FTransform", Ref);
		}
		
	}
}

void URenderUnit::ConstantBufferLinkData(std::string_view _Name, void* _Data)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsConstantBuffer(_Name))
		{
			continue;
		}

		Resources[i].ConstantBufferLinkData(_Name, _Data);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, UEngineTexture* _Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsTexture(_Name))
		{
			continue;
		}

		Resources[i].TextureSetting(_Name, _Texture);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsTexture(_Name))
		{
			continue;
		}

		Resources[i].TextureSetting(_Name, _ResName);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsTexture(_Name))
		{
			continue;
		}

		Resources[i].TextureSetting(_Name, _Texture);
	}
}

void URenderUnit::SetSampler(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == Resources.contains(i))
		{
			continue;
		}

		if (false == Resources[i].IsSampler(_Name))
		{
			continue;
		}

		Resources[i].SamplerSetting(_Name, _ResName);
	}
}

void URenderUnit::SetMesh(std::string_view _Name)
{
	Mesh = UMesh::Find<UMesh>(_Name);

	if (nullptr == Mesh)
	{
		MSGASSERT("존재하지 않는 매쉬를 세팅하려고 했습니다.");
	}

	if (nullptr != Material)
	{
		InputLayOutCreate();
	}
}

void URenderUnit::SetMaterial(std::string_view _Name)
{
	Material = UEngineMaterial::Find<UEngineMaterial>(_Name);

	if (nullptr == Material)
	{
		MSGASSERT("존재하지 않는 머티리얼을를 세팅하려고 했습니다.");
	}

	MaterialResourcesCheck();

	if (nullptr != Mesh)
	{
		InputLayOutCreate();
	}
}

void URenderUnit::Render(class UEngineCamera* _Camera, float _DeltaTime)
{
	for (std::pair<const EShaderType, UEngineShaderResources>& Pair : Resources)
	{
		Pair.second.Setting();
	}

	//	InputAssembler1Setting();
	Mesh->GetVertexBuffer()->Setting();

	//	VertexShaderSetting();
	Material->GetVertexShader()->Setting();
	
	//	InputAssembler2Setting();
	Mesh->GetIndexBuffer()->Setting();
	Material->PrimitiveTopologySetting();

	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayOut.Get());

	//	RasterizerSetting();
	Material->GetRasterizerState()->Setting();

	//	PixelShaderSetting();
	Material->GetPixelShader()->Setting();

	//	OutPutMergeSetting();
	
	Material->GetBlend()->Setting();

	Material->GetDepthStencilState()->Setting();

	UEngineCore::GetDevice().GetContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderUnit::InputLayOutCreate()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();

	UEngineInputLayOutInfo* InfoPtr = Mesh->GetVertexBuffer()->GetInfoPtr();

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InfoPtr->InputLayOutData[0],
		static_cast<unsigned int>(InfoPtr->InputLayOutData.size()),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayOut);
}