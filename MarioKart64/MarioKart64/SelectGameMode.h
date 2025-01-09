#pragma once
#include <EngineCore/GameMode.h>

class SelectGameMode : public AGameMode
{
public:
	enum class Scene
	{
		IDLE = 0,
		SELECT_GAME,
		SELECT_CHARACTER,
		SELECT_MAP,
		END,
	};

	SelectGameMode();
	~SelectGameMode();

	SelectGameMode(const SelectGameMode& _other) = delete;
	SelectGameMode(SelectGameMode&& _other) noexcept = delete;
	SelectGameMode& operator=(const SelectGameMode& _other) = delete;
	SelectGameMode& operator=(SelectGameMode&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	Scene SceneIdx = Scene::IDLE;
};
