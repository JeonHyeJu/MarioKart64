#pragma once
#include "BlinkActor.h"

class USpriteRenderer;
class AGameSelectBox : public ABlinkActor
{
public:
	enum class OptionType
	{
		MARIO_GP = 0,
		TIME_TRAIALS,
		VS,
		BATTLE,
		END
	};

	AGameSelectBox();
	~AGameSelectBox();

	AGameSelectBox(const AGameSelectBox& _other) = delete;
	AGameSelectBox(AGameSelectBox&& _other) noexcept = delete;
	AGameSelectBox& operator=(const AGameSelectBox& _other) = delete;
	AGameSelectBox& operator=(AGameSelectBox&& _other) noexcept = delete;

	void Init(int _idx);
	void GetRectSize(float* _refW, float* _refH);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void CreateOptionList(const std::vector<OptionType>& _options);

	std::shared_ptr<USpriteRenderer> RRect = nullptr;
	std::shared_ptr<USpriteRenderer> RDownArrow = nullptr;
	std::list<std::shared_ptr<class ASelectButton>> RBtnOptions;
	//std::list<class ASelectButton*> RBtnOptions;
};
