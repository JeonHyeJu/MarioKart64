#pragma once
#include "EngineResources.h"
#include "EngineTexture.h"
#include "RenderUnit.h"
#include "EngineGraphicDevice.h"

class UEngineRenderTarget : public UEngineResources
{
public:
	UEngineRenderTarget();
	~UEngineRenderTarget();

	UEngineRenderTarget(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget(UEngineRenderTarget&& _Other) noexcept = delete;
	UEngineRenderTarget& operator=(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget& operator=(UEngineRenderTarget&& _Other) noexcept = delete;

	ENGINEAPI void CreateTarget(float4 _Scale, float4 _ClearColor = float4::NONE, DXGI_FORMAT _Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);

	ENGINEAPI void CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D);

	ENGINEAPI void CreateDepth(int _Index = 0);

	ENGINEAPI void Clear();

	ENGINEAPI void Setting();

	ENGINEAPI void SetClearColor(float4 _ClearColor)
	{
		ClearColor = _ClearColor;
	}

	ENGINEAPI void CopyTo(std::shared_ptr<UEngineRenderTarget> _Target);

	ENGINEAPI void MergeTo(std::shared_ptr<UEngineRenderTarget> _Target);

protected:

private:
	float4 ClearColor = float4(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<std::shared_ptr<class UEngineTexture>> ArrTexture;
	std::vector<ID3D11RenderTargetView*> ArrRTVs;
	std::vector<ID3D11ShaderResourceView*> ArrSRVs;

	std::shared_ptr<class UEngineTexture> DepthTexture;

	URenderUnit TargetUnit;
};

