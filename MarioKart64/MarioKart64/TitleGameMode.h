#pragma once
#include <EngineCore/Actor.h>

class ATitleGameMode : public AActor
{
public:
	enum class SceneNum
	{
		IDLE = 0,
		NINTENDO_LOGO,
		TITLE,
		END,
	};

	ATitleGameMode();
	~ATitleGameMode();

	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void CheckKey();
	void ShowScene(SceneNum _sceneNum);

	std::shared_ptr<class ANintendoLogo> NintendoLogo = nullptr;
	std::shared_ptr<class ATitle> Title = nullptr;

	SceneNum SceneIdx = SceneNum::IDLE;
};

