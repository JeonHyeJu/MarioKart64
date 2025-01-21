#pragma once
#include <EngineCore/Actor.h>

class USpriteRenderer;
class ACharacterAndName : public AActor
{
public:
	ACharacterAndName();
	~ACharacterAndName();

	ACharacterAndName(const ACharacterAndName& _other) = delete;
	ACharacterAndName(ACharacterAndName&& _other) noexcept = delete;
	ACharacterAndName& operator=(const ACharacterAndName& _other) = delete;
	ACharacterAndName& operator=(ACharacterAndName&& _other) noexcept = delete;

	void Init(std::string_view _spriteName, int _idx, float _scaleRatio);
	void SetBlink();
	void SetSelect();
	void SetIdle();
	void SetEnable(bool _val);
	FVector GetScale();
	FVector GetScaleName();

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<USpriteRenderer> RImage = nullptr;
	std::shared_ptr<USpriteRenderer> RName = nullptr;
	std::shared_ptr<class ColorRenderer> RDisable = nullptr;
};
