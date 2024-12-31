#pragma once
#include <EngineCore/Actor.h>

class ATestGameMode : public AActor
{
public:
	ATestGameMode();
	~ATestGameMode();

	ATestGameMode(const ATestGameMode& _Other) = delete;
	ATestGameMode(ATestGameMode&& _Other) noexcept = delete;
	ATestGameMode& operator=(const ATestGameMode& _Other) = delete;
	ATestGameMode& operator=(ATestGameMode&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);

protected:

private:
	std::shared_ptr<class TestActor> Logo;
};

