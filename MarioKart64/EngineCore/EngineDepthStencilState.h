#pragma once
#include "EngineResources.h"

class UEngineDepthStencilState : public UEngineResources
{
public:
	UEngineDepthStencilState();
	~UEngineDepthStencilState();

	UEngineDepthStencilState(const UEngineDepthStencilState& _Other) = delete;
	UEngineDepthStencilState(UEngineDepthStencilState&& _Other) noexcept = delete;
	UEngineDepthStencilState& operator=(const UEngineDepthStencilState& _Other) = delete;
	UEngineDepthStencilState& operator=(UEngineDepthStencilState&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineDepthStencilState> Create(std::string_view _Name, const D3D11_DEPTH_STENCIL_DESC& _Value);

	void Setting();

protected:
	void ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> State = nullptr;
};

