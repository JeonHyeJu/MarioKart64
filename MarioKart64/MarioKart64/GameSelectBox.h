#pragma once
#include "BlinkActor.h"

class USpriteRenderer;
class AGameSelectBox : public ABlinkActor
{
public:
	enum class EOptionType
	{
		MARIO_GP = 0,
		TIME_TRAIALS,
		VS,
		BATTLE,
		END
	};

	enum class ECCType
	{
		C50 = 50,
		C100 = 100,
		C150 = 150,
	};

	AGameSelectBox();
	~AGameSelectBox();

	AGameSelectBox(const AGameSelectBox& _other) = delete;
	AGameSelectBox(AGameSelectBox&& _other) noexcept = delete;
	AGameSelectBox& operator=(const AGameSelectBox& _other) = delete;
	AGameSelectBox& operator=(AGameSelectBox&& _other) noexcept = delete;

	void Init(int _idx);
	void GetRectSize(float* _refW, float* _refH);
	void SetActiveCCList(bool _val);
	void SetBlinkStateToRuleList(int _idx, EBlinkState _state=EBlinkState::OFF);
	void SetBlinkStateToCCList(int _idx, EBlinkState _state=EBlinkState::OFF);
	void UpBlinkStateToRuleList();
	void DownBlinkStateToRuleList();
	void UpBlinkStateToCCList();
	void DownBlinkStateToCCList();

	uint8_t GetSelectedIdxRule() const
	{
		return SelectedIdxRule;
	}
	uint8_t GetSelectedIdxCC() const
	{
		return SelectedIdxCC;
	}

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void CreateRuleList(const std::vector<EOptionType>& _options);
	void CreateCCList(const std::vector<ECCType>& _options);

	std::shared_ptr<USpriteRenderer> RRect = nullptr;
	std::shared_ptr<USpriteRenderer> RDownArrow = nullptr;
	std::vector<class ASelectButton*> ARuleList;
	std::vector<class ASelectButton*> ACCList;

	uint8_t SelectedIdxRule = 0;
	uint8_t SelectedIdxCC = 0;
};
