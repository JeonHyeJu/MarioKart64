#pragma once
#include "BaseMap.h"

class ASherbetLand : public ABaseMap
{
public:
	ASherbetLand();
	~ASherbetLand();

	ASherbetLand(const ASherbetLand& _other) = delete;
	ASherbetLand(ASherbetLand&& _other) noexcept = delete;
	ASherbetLand& operator=(const ASherbetLand& _other) = delete;
	ASherbetLand& operator=(ASherbetLand&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 8;
};
