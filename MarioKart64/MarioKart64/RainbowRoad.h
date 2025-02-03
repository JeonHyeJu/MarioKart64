#pragma once
#include "BaseMap.h"

class ARainbowRoad : public ABaseMap
{
public:
	ARainbowRoad();
	~ARainbowRoad();

	ARainbowRoad(const ARainbowRoad& _other) = delete;
	ARainbowRoad(ARainbowRoad&& _other) noexcept = delete;
	ARainbowRoad& operator=(const ARainbowRoad& _other) = delete;
	ARainbowRoad& operator=(ARainbowRoad&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 6;
};
