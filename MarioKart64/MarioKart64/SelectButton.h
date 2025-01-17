#pragma once
#include "BlinkActor.h"
#include <string>

class ASelectButton : public ABlinkActor
{
public:
	ASelectButton();
	~ASelectButton();

	ASelectButton(const ASelectButton& _other) = delete;
	ASelectButton(ASelectButton&& _other) noexcept = delete;
	ASelectButton& operator=(const ASelectButton& _other) = delete;
	ASelectButton& operator=(ASelectButton&& _other) noexcept = delete;

	void Init(std::string_view _name, UINT idx, float _scale=1.f);
	void SetImage(std::string_view _name, UINT idx);
	FVector GetRealScaleOfSprite() const;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class USpriteRenderer> RBtn = nullptr;
};
