#pragma once
#include "BlinkActor.h"

struct SButtonData
{
	std::string SpriteName = "";
	int SpriteIndex = 0;
	float AutoScaleRatio = 1.f;
	FVector InitLoc = FVector::ZERO;
	bool IsInitActive = true;
};

enum class ELayer
{
	RULE,
	CC,
	END
};

enum class EDirection
{
	DOWN = -1,
	UP = 1
};

class USpriteRenderer;
class AGameSelectBox : public ABlinkActor
{
public:
	AGameSelectBox();
	~AGameSelectBox();

	AGameSelectBox(const AGameSelectBox& _other) = delete;
	AGameSelectBox(AGameSelectBox&& _other) noexcept = delete;
	AGameSelectBox& operator=(const AGameSelectBox& _other) = delete;
	AGameSelectBox& operator=(AGameSelectBox&& _other) noexcept = delete;

	void Init(int _idx);
	void SetActiveToLayers(ELayer _idx, bool _val);
	void SetBlinkStateToLayer(ELayer _layerIdx, int _idx, EBlinkState _state = EBlinkState::OFF);
	void MoveSelectedLayer(ELayer _idx, EDirection _dir);

	void GetRectSize(float* _refW, float* _refH);
	FVector GetButtonSize(ELayer _layerIdx, int _idx=0);

	uint8_t GetSelectedIdx(ELayer _idx)
	{
		return SelectedIdxs[_idx];
	}

	void AddObjectsToLayer(ELayer _idx, const std::vector<SButtonData>& _options);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<USpriteRenderer> RRect = nullptr;
	std::shared_ptr<USpriteRenderer> RDownArrow = nullptr;

	std::map<ELayer, std::vector<class ASelectButton*>> Buttons;
	std::map<ELayer, uint8_t> SelectedIdxs;
};
