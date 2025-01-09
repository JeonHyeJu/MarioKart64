#pragma once
#include <EngineCore/GameMode.h>

class ATitleGameMode : public AGameMode
{
public:
	enum class Scene
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
	void SpinLogoAndTimeCheck(float _deltaTime);
	void ShowScene(Scene _sceneNum);

	std::shared_ptr<class ANintendoLogo> NintendoLogo = nullptr;
	std::shared_ptr<class ATitle> Title = nullptr;

	Scene SceneIdx = Scene::IDLE;
};

