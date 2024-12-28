#pragma once
#include <EngineCore/Actor.h>

class APlayGameMode : public AActor
{
public:
	APlayGameMode();
	~APlayGameMode();

	APlayGameMode(const APlayGameMode& _other) = delete;
	APlayGameMode(APlayGameMode&& _other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<class ATestMap> TestMapPtr = nullptr;
};
