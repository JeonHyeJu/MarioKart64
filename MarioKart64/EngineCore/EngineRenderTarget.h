#pragma once
#include "EngineResources.h"
#include "EngineTexture.h"
#include "RenderUnit.h"
#include "EngineGraphicDevice.h"

class UPostEffect
{
	friend class UEngineRenderTarget;

public:
	URenderUnit RenderUnit;
	UEngineRenderTarget* ResultTarget;

	bool IsActive = true;

protected:
	ENGINEAPI virtual void Init() = 0
	{

	}
	ENGINEAPI virtual void Effect(UEngineCamera* Camera, float _DeltaTime) = 0
	{

	}
private:
};

class UEngineRenderTarget : public UEngineResources
{
public:
	ENGINEAPI UEngineRenderTarget();
	ENGINEAPI ~UEngineRenderTarget();

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

	ENGINEAPI void CopyTo(UEngineRenderTarget* _Target);
	ENGINEAPI void MergeTo(UEngineRenderTarget* _Target);

	UEngineTexture* GetTexture(int _Index = 0)
	{
		return ArrTexture[_Index].get();
	}

protected:

private:
	float4 ClearColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<std::shared_ptr<class UEngineTexture>> ArrTexture;
	std::vector<ID3D11RenderTargetView*> ArrRTVs;
	std::vector<ID3D11ShaderResourceView*> ArrSRVs;

	std::shared_ptr<class UEngineTexture> DepthTexture;

	URenderUnit TargetUnit;

public:
	template<typename EffectType>
	void AddEffect()
	{
		std::shared_ptr<EffectType> NewEffect = std::make_shared<EffectType>();

		std::shared_ptr<UPostEffect> PostEffect = std::dynamic_pointer_cast<UPostEffect>(NewEffect);

		PostEffect->ResultTarget = this;
		PostEffect->Init();
		PosEffects.push_back(NewEffect);
	}

	void Effect(UEngineCamera* _Camera, float _DeltaTime);
	std::shared_ptr<UPostEffect> GetPostEffect(int _Index)
	{
		return PosEffects[_Index];
	}

private:
	std::vector<std::shared_ptr<UPostEffect>> PosEffects;
};

