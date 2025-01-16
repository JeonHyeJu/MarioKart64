#pragma once
#include "EngineResources.h"
#include "EngineEnums.h"

class UEngineSampler : public UEngineResources
{
public:
	UEngineSampler();
	~UEngineSampler();

	UEngineSampler(const UEngineSampler& _Other) = delete;
	UEngineSampler(UEngineSampler&& _Other) noexcept = delete;
	UEngineSampler& operator=(const UEngineSampler& _Other) = delete;
	UEngineSampler& operator=(UEngineSampler&& _Other) noexcept = delete;


	static std::shared_ptr<UEngineSampler> Create(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value);

	void Setting(EShaderType _Type, UINT _BindIndex);

	void Reset(EShaderType _Type, UINT _BindIndex);

protected:
	void ResCreate(const D3D11_SAMPLER_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> State = nullptr;

};

