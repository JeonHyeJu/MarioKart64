#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/FSMStateManager.h>

class ASelectButton;
class AGameSelectBox;
class USpriteRenderer;
class ASelectGame : public AActor
{
public:
	enum class ESceneState
	{
		SELECT_GAME = 0,
		CHANGE_GAME_TO_RULE,
		CHANGE_RULE_TO_GAME,
		SELECT_RULE,
		SELECT_CC,
		WAIT_OK,
		FINISH,
		END
	};

	ASelectGame();
	~ASelectGame();

	ASelectGame(const ASelectGame& _other) = delete;
	ASelectGame(ASelectGame&& _other) noexcept = delete;
	ASelectGame& operator=(const ASelectGame& _other) = delete;
	ASelectGame& operator=(ASelectGame&& _other) noexcept = delete;

	void SetCallback(std::function<void()> _fn)
	{
		EndFuntion = _fn;
	}

	uint8_t GetSelectedIdx() const
	{
		return SelectedGameIdx;
	}

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void InitGameSelectBox();
	void SwitchNoSelectedGameBoxes(bool _isVisible);
	void SetIndex(int _idx);

	/* Fsm start function */
	void OnSelectCC();
	void OnWaitOk();
	void OnFinish();

	/* Fsm update function */
	void SelectingGame(float _deltaTime);
	void ChangingGameToRule(float _deltaTime);
	void ChangingRuleToGame(float _deltaTime);
	void SelectingRule(float _deltaTime);
	void SelectingCC(float _deltaTime);
	void WaitingOk(float _deltaTime);

	std::shared_ptr<USpriteRenderer> RBackground = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	std::shared_ptr<ASelectButton> RBtnOption = nullptr;
	std::shared_ptr<ASelectButton> RBtnData = nullptr;
	std::shared_ptr<ASelectButton> RBtnOk = nullptr;

	static const int SELECT_LIST_SIZE = 4;
	AGameSelectBox* SelectBoxes[SELECT_LIST_SIZE] = { nullptr, };
	std::shared_ptr<AGameSelectBox> Tester = nullptr;
	AGameSelectBox* PtrSelectedBox = nullptr;

	float AddRectDist = 0.f;
	uint8_t SelectedGameIdx = 0;
	FVector LocOrgSelectedBox;
	int DirMoveSelectedBox = 1;

	std::function<void()> EndFuntion;

	UFSMStateManager Fsm;
};
