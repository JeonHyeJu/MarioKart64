#pragma once
#include <EngineCore/Actor.h>
#include <EngineBase/FSMStateManager.h>

class USpriteRenderer;
class ASelectMap : public AActor
{
public:
	enum class ESceneState
	{
		SELECT_MAP = 0,
		WAIT_OK,
		FINISH,
		END
	};

	ASelectMap();
	~ASelectMap();

	ASelectMap(const ASelectMap& _other) = delete;
	ASelectMap(ASelectMap&& _other) noexcept = delete;
	ASelectMap& operator=(const ASelectMap& _other) = delete;
	ASelectMap& operator=(ASelectMap&& _other) noexcept = delete;

	void SetCallback(std::function<void()> _fn)
	{
		EndFuntion = _fn;
	}

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void Hover(uint8_t _idx);

	/* Fsm start function */
	void OnFinish();
	void OnShowSelectMap();
	void OnWaitOk();

	/* Fsm update function */
	void Selecting(float _deltaTime);
	void WaitingOk(float _deltaTime);

	static const int SIZE = 4;
	std::shared_ptr<USpriteRenderer> RBg = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	class ASelectButton* MainRects[4] = { nullptr, };
	std::shared_ptr<class AMapSpec> MapSpec = nullptr;
	std::shared_ptr<class ASelectButton> BtnOk = nullptr;


	uint8_t PreSelectedIdx = 0;
	uint8_t CurSelectedIdx = 0;

	std::function<void()> EndFuntion;

	UFSMStateManager Fsm;
};
