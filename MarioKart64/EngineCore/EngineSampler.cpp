#include "PreCompile.h"
#include "EngineSampler.h"
#include "EngineCore.h"

UEngineSampler::UEngineSampler()
{
}

UEngineSampler::~UEngineSampler()
{
}

std::shared_ptr<UEngineSampler> UEngineSampler::Create(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineSampler> NewRes = std::make_shared<UEngineSampler>();
	PushRes<UEngineSampler>(NewRes, _Name, "");
	NewRes->ResCreate(_Value);

	return NewRes;
}

void UEngineSampler::ResCreate(const D3D11_SAMPLER_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
	
}

void UEngineSampler::Setting(EShaderType _Type, UINT _BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { State.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("아직 존재하지 않는 쉐이더에 세팅하려고 했습니다.");
		break;
	}
}
