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

	void StartLuigiRaceway();
	void StartRoyalRaceway();

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class ASkybox> Skybox = nullptr;
	std::shared_ptr<class ABaseMap> MapPtr = nullptr;
	//class ADriver* Player = nullptr;
	class APlayer* Player = nullptr;

	std::shared_ptr<class AItemBox> TestItemBox = nullptr;
};
