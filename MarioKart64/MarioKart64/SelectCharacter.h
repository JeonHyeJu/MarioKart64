#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/FSMStateManager.h>

class USpriteRenderer;
class ASelectCharacter : public AActor
{
public:
	enum class ESceneState
	{
		SELECT = 0,
		SELECT_MOVING,
		SELECT_MOVING_REVERSE,
		WAIT_OK,
		FINISH,
		END
	};

	ASelectCharacter();
	~ASelectCharacter();

	ASelectCharacter(const ASelectCharacter& _other) = delete;
	ASelectCharacter(ASelectCharacter&& _other) noexcept = delete;
	ASelectCharacter& operator=(const ASelectCharacter& _other) = delete;
	ASelectCharacter& operator=(ASelectCharacter&& _other) noexcept = delete;

	void SetCallback(std::function<void()> _fn)
	{
		EndFuntion = _fn;
	}

	uint8_t GetSelectedIdx() const
	{
		return SelectedIdx;
	}

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void InitCharacterImgs();
	void RunBlink(int _idx);
	void MoveSelectUI(int _idx);
	void OnOffObjs(bool _isActive);

	/* Fsm start functions */
	void OnSelect();
	void OnSelectMoveForward();
	void OnSelectMoveBackward();
	void OnWaitOk();
	void OnFinish();

	/* Fsm update functions */
	void Selecting(float _deltaTime);
	void Moving(float _deltaTime);
	void Waiting(float _deltaTime);

	static const int SIZE = 8;
	const int HALF_SIZE = SIZE / 2;
	uint8_t SelectedIdx = 0;
	const float MARGIN_Y = 50.f;
	FVector SelectMargin = FVector::ZERO;
	int DISABLE_IDXS[2] = { 3, 5 };

	FVector LocOrg;
	FVector LocDst;

	std::shared_ptr<USpriteRenderer> RBg = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	std::shared_ptr<USpriteRenderer> RSelect = nullptr;
	std::shared_ptr<USpriteRenderer> RPlayer1 = nullptr;
	std::shared_ptr<class ASelectButton> BtnOk = nullptr;
	std::shared_ptr<class ACharacterAndName> ImageList[SIZE] = { nullptr, };

	std::function<void()> EndFuntion;
	
	UFSMStateManager Fsm;
};
