#pragma once
#include <EngineCore/EngineRenderTarget.h>

class FxShrinkEffect : public UPostEffect
{
public:
	FxShrinkEffect();
	~FxShrinkEffect();

	FxShrinkEffect(const FxShrinkEffect& _other) = delete;
	FxShrinkEffect(FxShrinkEffect&& _other) noexcept = delete;
	FxShrinkEffect& operator=(const FxShrinkEffect& _other) = delete;
	FxShrinkEffect& operator=(FxShrinkEffect&& _other) noexcept = delete;

protected:
	void Init() override;
	void Effect(UEngineCamera* _camera, float _deltaTime) override;

private:
	std::shared_ptr<UEngineRenderTarget> CopyTarget;
};
