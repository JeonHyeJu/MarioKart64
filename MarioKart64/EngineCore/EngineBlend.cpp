#include "PreCompile.h"
#include "EngineBlend.h"
#include "EngineCore.h"

UEngineBlend::UEngineBlend()
{
}

UEngineBlend::~UEngineBlend()
{
}

std::shared_ptr<UEngineBlend> UEngineBlend::Create(std::string_view _Name, const D3D11_BLEND_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineBlend> NewRes = std::make_shared<UEngineBlend>();
	PushRes<UEngineBlend>(NewRes, _Name, "");
	NewRes->ResCreate(_Value);

	return NewRes;
}


void UEngineBlend::ResCreate(const D3D11_BLEND_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateBlendState(&_Value, &State))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
	
}

void UEngineBlend::Setting()
{
	UEngineCore::GetDevice().GetContext()->OMSetBlendState(State.Get(), BlendFactor.Arr1D, Mask);
}
