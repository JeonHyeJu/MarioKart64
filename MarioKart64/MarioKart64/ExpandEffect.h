#pragma once
#include <EngineCore/EngineRenderTarget.h>

class FxExpandEffect : public UPostEffect
{
public:
	FxExpandEffect();
	~FxExpandEffect();

	FxExpandEffect(const FxExpandEffect& _other) = delete;
	FxExpandEffect(FxExpandEffect&& _other) noexcept = delete;
	FxExpandEffect& operator=(const FxExpandEffect& _other) = delete;
	FxExpandEffect& operator=(FxExpandEffect&& _other) noexcept = delete;

protected:
	void Init() override;
	void Effect(UEngineCamera* _camera, float _deltaTime) override;

private:
	std::shared_ptr<UEngineRenderTarget> CopyTarget;
};
