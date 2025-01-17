#pragma once
#include <EngineCore/Actor.h>

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

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void RunBlink(int _idx);
	void Move(int _idx);
	void OffObjs();

	void Selecting(float _deltaTime);
	void Moving(float _deltaTime);
	void Waiting(float _deltaTime);
	void OnFinish();

	static const int SIZE = 8;
	const int HALF_SIZE = SIZE / 2;
	int SelectedIdx = 0;
	const float MARGIN_Y = 80.f;	// Temp

	FVector LocOrg;
	FVector LocOrgName;

	FVector LocDst;

	std::shared_ptr<USpriteRenderer> RBg = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	std::shared_ptr<USpriteRenderer> RSelect = nullptr;
	std::shared_ptr<USpriteRenderer> RPlayer1 = nullptr;
	std::shared_ptr<class ASelectButton> BtnOk = nullptr;
	std::shared_ptr<USpriteRenderer> ImageList[SIZE] = { nullptr, };
	std::shared_ptr<USpriteRenderer> NameList[SIZE] = { nullptr, };

	std::function<void()> EndFuntion;
	
	ESceneState SceneState = ESceneState::SELECT;
};
