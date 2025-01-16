#pragma once
#include <EngineCore/Actor.h>

class ABlinkActor : public AActor
{
public:
	ABlinkActor();
	~ABlinkActor();

	ABlinkActor(const ABlinkActor& _other) = delete;
	ABlinkActor(ABlinkActor&& _other) noexcept = delete;
	ABlinkActor& operator=(const ABlinkActor& _other) = delete;
	ABlinkActor& operator=(ABlinkActor&& _other) noexcept = delete;

	void SetSelected(bool _val);
	void SetBgScale(const FVector& _scale);
	void SetBgLocation(const FVector& _loc);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void ChangeSelectedRectColor(float _deltaTime);

	std::shared_ptr<class ColorRenderer> RBackground = nullptr;

	bool IsSelected = false;
	float RectBgVal = 1.f;
	const FVector OFF_COLOR = { 0.f, 0.f, 0.f, 1.f };
};
