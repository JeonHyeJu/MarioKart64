#pragma once
#include "BaseMap.h"

class ALuigiRaceway : public ABaseMap
{
public:
	ALuigiRaceway();
	~ALuigiRaceway();

	ALuigiRaceway(const ALuigiRaceway& _other) = delete;
	ALuigiRaceway(ALuigiRaceway&& _other) noexcept = delete;
	ALuigiRaceway& operator=(const ALuigiRaceway& _other) = delete;
	ALuigiRaceway& operator=(ALuigiRaceway&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 4;
};
