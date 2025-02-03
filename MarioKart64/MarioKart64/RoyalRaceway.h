#pragma once
#include "BaseMap.h"

class ARoyalRaceway : public ABaseMap
{
public:
	ARoyalRaceway();
	~ARoyalRaceway();

	ARoyalRaceway(const ARoyalRaceway& _other) = delete;
	ARoyalRaceway(ARoyalRaceway&& _other) noexcept = delete;
	ARoyalRaceway& operator=(const ARoyalRaceway& _other) = delete;
	ARoyalRaceway& operator=(ARoyalRaceway&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 7;
};
