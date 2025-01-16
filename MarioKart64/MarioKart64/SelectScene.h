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
		CHANGE_GAME_TO_RULE,
		CHANGE_RULE_TO_GAME,
		SELECT_RULE,
		SELECT_CC,
		END
	};

	ASelectScene();
	~ASelectScene();

	ASelectScene(const ASelectScene& _other) = delete;
	ASelectScene(ASelectScene&& _other) noexcept = delete;
	ASelectScene& operator=(const ASelectScene& _other) = delete;
	ASelectScene& operator=(ASelectScene&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void ChangingGameToRule(float _deltaTime);
	void ChangingRuleToGame(float _deltaTime);
	void SwitchNoSelectedGameBoxes(bool _isVisible);

	// Fsm
	void SelectingGame(float _deltaTime);
	void SelectingRule(float _deltaTime);
	void SelectingCC(float _deltaTime);

	std::shared_ptr<USpriteRenderer> RBackground = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	std::shared_ptr<ASelectButton> RBtnOption = nullptr;
	std::shared_ptr<ASelectButton> RBtnData = nullptr;

	static const int SELECT_LIST_SIZE = 4;
	AGameSelectBox* SelectBoxes[SELECT_LIST_SIZE] = { nullptr, };
	std::shared_ptr<AGameSelectBox> Tester = nullptr;

	float AddRectDist = 0.f;
	int SelectedGameIdx = 0;
	FVector LocOrgSelectedBox;
	int DirMoveSelectedBox = 1;

	SceneState State = SceneState::SELECT_GAME;
};
