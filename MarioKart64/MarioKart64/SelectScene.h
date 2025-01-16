#pragma once
#include <EngineCore/Actor.h>

class ASelectButton;
class AGameSelectBox;
class USpriteRenderer;
class ASelectScene : public AActor
{
public:
	enum class SceneState
	{
		SELECT_GAME = 0,
		CHANGE_GAME_TO_CC,
		SELECT_CC,
		END
	};

	ASelectScene();
	~ASelectScene();

	ASelectScene(const ASelectScene& _other) = delete;
	ASelectScene(ASelectScene&& _other) noexcept = delete;
	ASelectScene& operator=(const ASelectScene& _other) = delete;
	ASelectScene& operator=(ASelectScene&& _other) noexcept = delete;

	void SelectingGame(float _deltaTime);
	void ChangingGameToCC(float _deltaTime);
	void SelectingCC(float _deltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<USpriteRenderer> RBackground = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	std::shared_ptr<ASelectButton> RBtnOption = nullptr;
	std::shared_ptr<ASelectButton> RBtnData = nullptr;

	static const int SELECT_LIST_SIZE = 4;
	AGameSelectBox* SelectBoxes[SELECT_LIST_SIZE] = { nullptr, };
	std::shared_ptr<AGameSelectBox> Tester = nullptr;

	float AddRectDist = 0.f;
	int SelectedGameIdx = 0;

	SceneState State = SceneState::SELECT_GAME;
};
