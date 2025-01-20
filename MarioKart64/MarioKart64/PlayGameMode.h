#pragma once
#include <EngineCore/GameMode.h>

class APlayGameMode : public AGameMode
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
	void Tick(float _deltaTime) override;

private:
	void CheckCollision(float _deltaTime);
	void CheckCollisionOfAllMap();

	std::shared_ptr<class ASkybox> Skybox = nullptr;
	std::shared_ptr<class ATestMap> TestMapPtr = nullptr;
	std::shared_ptr<class ADriver> Player = nullptr;
	//std::shared_ptr<class APlayer> Player = nullptr;

	std::shared_ptr<class AItemBox> TestItemBox = nullptr;
};
