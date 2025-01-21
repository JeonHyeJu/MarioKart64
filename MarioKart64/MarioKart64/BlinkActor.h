#pragma once
#include <EngineCore/Actor.h>

// TODO: FSM
enum class EBlinkState
{
	OFF = 0,
	BLINK,
	SELECTED,
	END
};

class ABlinkActor : public AActor
{
public:
	ABlinkActor();
	~ABlinkActor();

	ABlinkActor(const ABlinkActor& _other) = delete;
	ABlinkActor(ABlinkActor&& _other) noexcept = delete;
	ABlinkActor& operator=(const ABlinkActor& _other) = delete;
	ABlinkActor& operator=(ABlinkActor&& _other) noexcept = delete;

	void SetBlinkState(EBlinkState _state);
	void SetBgScale(const FVector& _scale);
	void SetBgLocation(const FVector& _loc);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void ChangeSelectedRectColor(float _deltaTime);

	std::shared_ptr<class ColorRenderer> RBackground = nullptr;

	float RectBgVal = 1.f;
	const FVector OFF_COLOR = FVector::BLACK;
	const FVector SEL_COLOR = FVector::WHITE;

	EBlinkState State = EBlinkState::OFF;
};
