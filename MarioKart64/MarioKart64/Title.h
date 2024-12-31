#pragma once
#include <EngineCore/Actor.h>

class ATitle : public AActor
{
public:
	ATitle();
	~ATitle();

	ATitle(const ATitle& _other) = delete;
	ATitle(ATitle&& _other) noexcept = delete;
	ATitle& operator=(const ATitle& _other) = delete;
	ATitle& operator=(ATitle&& _other) noexcept = delete;

	void Show();
	void Hide();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<class USpriteRenderer> RBackground = nullptr;
	std::shared_ptr<class USpriteRenderer> RTitleText = nullptr;
	std::shared_ptr<class USpriteRenderer> RPushButton = nullptr;
	std::shared_ptr<class USpriteRenderer> RCopyright = nullptr;

	std::shared_ptr<class ObjRenderer> RFlag = nullptr;

	bool IsHidden = false;	// Temp
};
